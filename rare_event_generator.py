import json
from time import sleep
import os
from pyverilog.vparser.parser import parse
from pyverilog.dataflow.dataflow_analyzer import VerilogDataflowAnalyzer
from pyverilog.utils.scope import ScopeChain
import csv
import re
import glob
from collections import defaultdict
import math
import pandas as pd
import matplotlib.pyplot as plt

###################################################################################
############## Step 1: Collect all the nets transitions from VCD text file ########
# Input: vcd text files
# Output: A single file with all the nets and their transition counts
# Currently not using in our framework but can be observed for understanding
###################################################################################
def collect_all_transitions_from_vcd_text(vcd_text_file_dir, file_lists, all_net_transitions_count):
    # Dictionary to store transition count per net
    net_transition_counts = defaultdict(int)

    # Loop over all matching log files
    for filename in glob.glob(vcd_text_file_dir + file_lists):
        print(f"Processing file: {filename}")
        with open(filename, "r") as file:
            for line in file:
                parts = line.strip().split(None, 2)
                if len(parts) == 3:
                    _, _, net = parts
                    net_transition_counts[net] += 1

    # Sort by transition count (descending)
    sorted_counts = sorted(net_transition_counts.items(), key=lambda x: x[1])

    # Save to file
    with open(all_net_transitions_count, "w") as out:
        for net, count in sorted_counts:
            out.write(f"{net} {count}\n")

    print(f"Saved sorted net transition counts to '{all_net_transitions_count}'")


###################################################################################
############## Step 2: Caculate signal probability from the vcd text file##########
# Input: single or multiple vcd text files
# Output: CSV file with p_hat_1, p_hat_0, R_v_min, toggles, steps, tau, is_bus
###################################################################################
def parse_int(val: str):
    s = val.strip().lower()
    if s in {"", "x", "z", "?"}:
        return None
    try:
        if s.startswith("0x"):
            return int(s, 16)
        if s.startswith("0b"):
            return int(s, 2)
        if re.search(r"[a-f]", s):  # bare hex like "ff"
            return int(s, 16)
        return int(s, 10)
    except ValueError:
        return None

def is_bus(name: str) -> bool:
    # treat as bus if it contains [msb:lsb] anywhere (e.g., foo(1)[19:0] also matches)
    return bool(re.search(r"\[\s*-?\d+\s*:\s*-?\d+\s*\]", name))

def suffix_after_anchor(fullname: str, anchor: str | None) -> str | None:
    if not anchor:
        return fullname
    idx = fullname.find(anchor)
    if idx < 0:
        return None
    cut = fullname[idx + len(anchor):]
    if cut.startswith("."):
        cut = cut[1:]
    return cut if cut else fullname

# --- Load events per net ---
def load_trace_file(path: str, anchor: str):
    per_net_events: dict[str, list[tuple[int,int|None]]] = defaultdict(list)
    min_cycle = None
    max_cycle = None
    try:
        with open(path, "r") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                parts = re.split(r"\s+", line)
                if len(parts) < 3:
                    continue
                cyc_s, val_s, net_full = parts[0], parts[1], parts[2]
                try:
                    cyc = int(cyc_s)
                except ValueError:
                    continue
                trimmed = suffix_after_anchor(net_full, anchor)
                if trimmed is None:
                    continue
                val = parse_int(val_s)
                per_net_events[trimmed].append((cyc, val))
                min_cycle = cyc if min_cycle is None else min(min_cycle, cyc)
                max_cycle = cyc if max_cycle is None else max(max_cycle, cyc)
    except FileNotFoundError:
        print(f"[WARN] File not found: {path}")
    return per_net_events, min_cycle, max_cycle

def build_dense_series(events: list[tuple[int,int|None]], start_cycle: int, end_cycle: int) -> list[int|None]:
    """Forward-fill after first known value, dense over [start_cycle, end_cycle]."""
    by_cycle = {}
    for c, v in events:
        by_cycle[c] = v  # last write wins at that cycle
    series = []
    last = None
    seen = False
    for c in range(start_cycle, end_cycle + 1):
        if c in by_cycle and by_cycle[c] is not None:
            last = by_cycle[c]
            seen = True
            series.append(last)
        else:
            series.append(last if seen else None)
    return series

def compute_bit_probs(series: list[int|None]) -> tuple[float,float,int]:
    """
    For single-bit nets: compute p_hat_1, p_hat_0 over dense samples
    AFTER first known value. We interpret non-zero as logic 1.
    """
    # drop leading None
    i = 0
    n = len(series)
    while i < n and series[i] is None:
        i += 1
    if i >= n:
        return 0.0, 1.0, 0
    vals = series[i:]
    total = len(vals)
    ones = sum(1 for v in vals if (v is not None and v != 0))
    p1 = ones / total
    p0 = 1.0 - p1
    return p1, p0, total

def compute_toggles_tau(series: list[int|None], compare_as_bit: bool) -> tuple[int,int,float]:
    """
    Count toggles across EVERY adjacent dense cycle from the first known value.
    compare_as_bit=True  -> compare 0/1 (for single-bit nets)
    compare_as_bit=False -> compare raw integer (for buses)
    Returns (toggles, steps, tau) with steps = last_cycle - first_seen_cycle (or 0 if unseen).
    """
    # find first known
    i = 0
    n = len(series)
    while i < n and series[i] is None:
        i += 1
    if i >= n:
        return 0, 0, 0.0

    def to_bit(v: int) -> int:
        return 0 if v == 0 else 1

    prev_raw = series[i]
    prev = to_bit(prev_raw) if compare_as_bit else prev_raw
    toggles = 0
    steps = 0
    for j in range(i + 1, n):
        cur_raw = series[j]
        if cur_raw is None:
            # With dense/hold semantics after first known, None shouldn't appear;
            # but if it does, treat as held value.
            cur_raw = prev_raw
        cur = to_bit(cur_raw) if compare_as_bit else cur_raw
        steps += 1
        if cur != prev:
            toggles += 1
        prev_raw = cur_raw
        prev = cur
    tau = (toggles / steps) if steps > 0 else 0.0
    return toggles, steps, tau



def compute_signal_probabilities_single_file(INPUT_PATH: str, ANCHOR: str, OUTPUT_CSV: str):
    """
    Compute signal probabilities and toggles from a single VCD text file.
    Returns a dictionary with net names as keys and their metrics as values.
    """
    results: dict[str, dict] = {}

    per_net_events, min_cycle, max_cycle = load_trace_file(INPUT_PATH, ANCHOR)

    for net, events in per_net_events.items():
        first_seen = min(c for c, _ in events)
        series = build_dense_series(events, first_seen, max_cycle)
        bus = is_bus(net)

        if bus:
            # Buses: no p-hat / Rv in output; rarity decided by tau
            p1 = p0 = None
            samples_dense = (max_cycle - first_seen + 1)
            toggles, steps, tau = compute_toggles_tau(series, compare_as_bit=False)
            # is_rare = (tau <= THETA) if steps > 0 else False
        else:
            # Single-bit nets: compute p-hat/Rv and tau; rarity decided by R_v_min
            p1, p0, samples_dense = compute_bit_probs(series)
            Rv_min = min(p1, p0)
            toggles, steps, tau = compute_toggles_tau(series, compare_as_bit=True)
            # is_rare = (Rv_min <= THETA) if samples_dense > 0 else False

        results[net] = {
            "net": net,
            "is_bus": bus,
            "first_seen_cycle": first_seen,
            "last_cycle": max_cycle,
            "samples_dense": samples_dense,
            "p_hat_1": p1 if not bus else None,
            "p_hat_0": p0 if not bus else None,
            "R_v_min": (min(p1, p0) if (p1 is not None and p0 is not None) else None),
            "toggles": toggles,
            "steps": steps,
            "tau": tau
            # "is_rare": is_rare,
        }
    # --- Write CSV from the dictionary ---
    fieldnames = [
        "net", "is_bus", "first_seen_cycle", "last_cycle", "samples_dense",
        "p_hat_1", "p_hat_0", "R_v_min", "toggles", "steps", "tau"
        #"is_rare"
    ]
    with open(OUTPUT_CSV, "w", newline="") as wf:
        w = csv.DictWriter(wf, fieldnames=fieldnames)
        w.writeheader()
        for net in sorted(results.keys(), key=lambda n: (not results[n]["is_bus"], n)):
            w.writerow(results[net])
    print(f"Wrote {len(results)} nets to {OUTPUT_CSV}")


def compute_signal_probabilities_multiple_file(INPUT_PATH: str, ANCHOR: str, OUTPUT_CSV: str):
    # ---- per-program metrics (list of dicts) ----
    program_metrics: list[dict[str, dict]] = []  # one dict per file: net -> metrics
    results: dict[str, dict] = {}
    for filepath in glob.glob(INPUT_PATH):
        print(f"Processing file: {filepath}")
        per_net_events, min_c, max_c = load_trace_file(filepath, ANCHOR)
        if not per_net_events or min_c is None or max_c is None:
            program_metrics.append({})  # keep place, but empty
            continue

        this_prog: dict[str, dict] = {}
        for net, events in per_net_events.items():
            bus = is_bus(net)
            first_seen = min(c for c, _ in events)
            series = build_dense_series(events, first_seen, max_c)

            if bus:
                # buses: toggles/steps/tau only
                toggles, steps, tau = compute_toggles_tau(series, compare_as_bit=False)
                metrics = {
                    "is_bus": True,
                    "samples_dense": (max_c - first_seen + 1),
                    "p1": None, "p0": None, "Rv": None,
                    "toggles": toggles,
                    "steps": steps,
                    "tau": tau,
                }
            else:
                p1, p0, samples = compute_bit_probs(series)
                toggles, steps, tau = compute_toggles_tau(series, compare_as_bit=True)
                metrics = {
                    "is_bus": False,
                    "samples_dense": samples,
                    "p1": p1, "p0": p0, "Rv": min(p1, p0),
                    "toggles": toggles,
                    "steps": steps,
                    "tau": tau,
                }
            this_prog[net] = metrics
        program_metrics.append(this_prog)

    # ---- aggregate across programs ----
    # Union of all nets:
    all_nets: set[str] = set()
    for pm in program_metrics:
        all_nets.update(pm.keys())

    for net in all_nets:
        # detect bus (prefer first occurrence that mentions it)
        bus_flag = None
        for pm in program_metrics:
            if net in pm:
                bus_flag = pm[net]["is_bus"]
                break
        if bus_flag is None:
            continue  # shouldn't happen

        # Initialize accumulators
        total_steps = 0
        total_toggles = 0
        total_samples = 0
        total_ones = 0.0  # for single-bit

        # Aggregate
        for pm in program_metrics:
            if net not in pm:
                continue
            m = pm[net]
            total_steps += m["steps"]
            total_toggles += m["toggles"]
            if not bus_flag:
                # single-bit: accumulate ones and samples to recompute p1 globally
                total_samples += m["samples_dense"]
                if m["p1"] is not None:
                    total_ones += m["p1"] * m["samples_dense"]

        # Compute aggregate metrics
        if bus_flag:
            tau_total = (total_toggles / total_steps) if total_steps > 0 else 0.0
            p1_total = None
            p0_total = None
            Rv_total = None
            # is_rare = (tau_total <= THETA) if total_steps > 0 else False
        else:
            p1_total = (total_ones / total_samples) if total_samples > 0 else 0.0
            p0_total = 1.0 - p1_total if total_samples > 0 else 1.0
            Rv_total = min(p1_total, p0_total) if total_samples > 0 else 0.0
            tau_total = (total_toggles / total_steps) if total_steps > 0 else 0.0
            # is_rare = (Rv_total <= THETA) if total_samples > 0 else False

        results[net] = {
            "net": net,
            "is_bus": bus_flag,
            "files_seen": sum(1 for pm in program_metrics if net in pm),
            "steps_total": total_steps,
            "toggles_total": total_toggles,
            "tau_total": tau_total,
            "p_hat_1": p1_total if not bus_flag else None,
            "p_hat_0": p0_total if not bus_flag else None,
            "R_v_min": Rv_total if not bus_flag else None
            # "is_rare": is_rare
        }
    # ---- write CSV from dictionary ----
    fieldnames = [
        "net", "is_bus", "files_seen",
        "steps_total", "toggles_total", "tau_total",
        "p_hat_1", "p_hat_0", "R_v_min"
        #"is_rare"
    ]

    with open(OUTPUT_CSV, "w", newline="") as wf:
        w = csv.DictWriter(wf, fieldnames=fieldnames)
        w.writeheader()
        for net in sorted(results.keys(), key=lambda n: (not results[n]["is_bus"], n)):
            w.writerow(results[net])

    print(f"Wrote {len(results)} nets to {OUTPUT_CSV}")

###################################################################################
############## Step 3: Choose Rare nets based on theta and  #######################
# ############ grouping them and create a new dict from existing CSV #############
###################################################################################

def rare_net_selection(INPUT_CSV: str, plot: bool = False):
    results_dict = {}
    ID = 1
    with open(INPUT_CSV, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            net_name = row["net"]

            # Convert numeric fields back from strings to proper types
            results_dict[net_name] = {
                "id": ID,
                "is_bus": row["is_bus"].lower() == "true",
                "toggles": int(row["toggles_total"]) if row["toggles_total"] else 0,
                "steps": int(row["steps_total"]) if row["steps_total"] else 0,
                "tau": float(row["tau_total"]) if row["tau_total"] else None,
                "p_hat_1": float(row["p_hat_1"]) if row["p_hat_1"] else None,
                "p_hat_0": float(row["p_hat_0"]) if row["p_hat_0"] else None,
                "R_v_min": float(row["R_v_min"]) if row["R_v_min"] else None
            }
            ID += 1


    # Extract ranges
    tau_values = [v["tau"] for v in results_dict.values() if v["tau"] is not None and v["toggles"] != 0 and v["is_bus"]]
    print("\n")
    print(f"Found {len(tau_values)} bus nets with non-zero τ values.")
    rvmin_values = [v["R_v_min"] for v in results_dict.values() if v["R_v_min"] is not None and v["toggles"] != 0 and not v[
        "is_bus"]]
    print(f"Found {len(rvmin_values)} single-bit nets with non-zero R_v_min values.")
    print(f"Maximum tau value: {max(tau_values):.10f} and minimum tau value: {min(tau_values):.10f}")
    print(f"Maximum R_v_min value: {max(rvmin_values):.10f} and minimum R_v_min value: {min(rvmin_values):.10f}")
    # Compute theta dynamically
    theta_tau = (max(tau_values) - min(tau_values)) * 0.10 if tau_values else None
    theta_rvmin = (max(rvmin_values) - min(rvmin_values)) * 0.10 if rvmin_values else None

    if not tau_values and not rvmin_values:
        raise RuntimeError("No non-zero τ or R_v_min values found after filtering.")

    # Unified theta range across both metrics
    theta_lo = min([x for x in ([min(tau_values)] if tau_values else []) +
                            ([min(rvmin_values)] if rvmin_values else [])])
    theta_hi = max([x for x in ([max(tau_values)] if tau_values else []) +
                            ([max(rvmin_values)] if rvmin_values else [])])

    print(f"Unified theta range: [{theta_lo:.10f}, {theta_hi:.10f}]")


    # Build the sweep: θ = θ_lo + p * (θ_hi - θ_lo), p in {0.1, 0.2, ..., 1.0}
    percentages = [i/20 for i in range(1, 21)]
    records = []

    for p in percentages:
        theta = theta_lo + p * (theta_hi - theta_lo)
        total = 0
        bus_count = 0
        bit_count = 0

        for net, e in results_dict.items():
            # Exclude nets with zero toggles or metric==0
            if e.get("toggles", 0) == 0:
                continue

            # Decide which metric applies
            if e.get("is_bus", True):
                val = e.get("tau")
            else:
                val = e.get("R_v_min")

            if val < theta:
                total += 1

        records.append({
            "percent": int(p*100),
            "theta": theta,
            "rare_total": total
        })

    # Tabulate and save (optional)
    df_theta = pd.DataFrame(records)
    print(df_theta)
    ################################################################################
    ################  Bar plot instead of line plot  ###############################
    ################################################################################
    if plot:
        plt.figure()
        plt.bar(df_theta["percent"], df_theta["rare_total"], width=3, label="Total rare nets")

        # Axis settings
        plt.xlabel("θ as % along unified range (min → max)")
        plt.ylabel("# nets with metric < θ (excluding zeros)")

        # Title and grid
        plt.title("Rare nets vs θ (single unified threshold across τ and R_v_min)")
        plt.grid(axis='y', linestyle='--', alpha=0.7)
        plt.legend()
        plt.show()
    return results_dict

########################################################################################
############### Step 4: Load the Verilog files and parse them ##########################
########################################################################################
def verilog2AST():
    verilog_files = [
        "mor1kx-defines.v",
        "mor1kx-sprs.v",
        "mor1kx_fetch_cappuccino.v",  # Top-level module
        "mor1kx_icache.v",
        "mor1kx_immu.v",
        "mor1kx_simple_dpram_sclk.v",
        "mor1kx_true_dpram_sclk.v",
        "mor1kx_store_buffer.v",
        "mor1kx_cache_lru.v",
    ]

    filelist = [
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_cpu_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_fetch_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_icache.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_dcache.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_immu.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_dmmu.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_simple_dpram_sclk.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_true_dpram_sclk.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_store_buffer.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_cache_lru.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_decode.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_decode_execute_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_branch_prediction.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_branch_predictor_simple.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_execute_alu.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_lsu_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_wb_mux_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_rf_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_execute_ctrl_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_ctrl_cappuccino.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_cfgrs.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_pic.v",
        "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_verilogs/mor1kx_ticktimer.v",

    ]
    topmodule = 'mor1kx_cpu_cappuccino'
    analyzer = VerilogDataflowAnalyzer(filelist, topmodule)
    analyzer.generate()
    terms = analyzer.getTerms()
    binddict = analyzer.getBinddict()
    return binddict
########################################################################################

def recursive_trace_json(AST, node, depth=0, visited=None, max_depth=4, history=None, flatten_parts=None,
                         decoder_stage=None):
    binddict = AST
    if visited is None:
        visited = set()
    if history is None:
        history = []
    if flatten_parts is None:
        flatten_parts = []
    if decoder_stage is None:
        decoder_stage = {"found": False}  # Use a dict to hold state
    if decoder_stage["found"]:
        return history, "DEPTH_LIMIT"
    if node is None or depth > max_depth:
        # print("  " * depth + f"Max depth {max_depth} reached.")
        return history, "DEPTH_LIMIT"

    for i, child in enumerate(node.children()):
        if node.__class__.__name__ == "DFBranch" and i == 1:
            # print(f"{indent} Skipping tracing of condition: {child.tocode()}")
            continue
        if hasattr(child, "tocode"):
            recursive_trace_json(AST, child, depth + 1, visited, max_depth, history, flatten_parts,
                                 decoder_stage=decoder_stage)

    # Leaf node reached
    if len(node.children()) == 0 and hasattr(node, "name"):
        leaf_signal = str(node.name)
        if leaf_signal in visited:
            return history, None
        visited.add(leaf_signal)

        # Check binddict for drivers of this leaf signal
        for k in binddict:
            if str(k).endswith(leaf_signal) and not "atomic" in str(k) and not "fpu" in str(k):
                module = str(k).split('.')[-2]
                for expr in binddict[k]:
                    if expr.tree:
                        expr_str = expr.tree.tocode()
                        if module == "mor1kx_decode":
                            decoder_stage["found"] = True  # Update mutable container
                        # Add to structured history
                        history.append({
                            "module": module,
                            "assign": f"{leaf_signal} = {expr_str}"
                        })
                        flatten_parts.append((leaf_signal, expr_str))
                        # Recurse on the newly found expression tree
                        recursive_trace_json(AST, expr.tree, depth + 1, visited, max_depth, history,
                                             decoder_stage=decoder_stage)

                    else:
                        history.append({
                            "module": module,
                            "port_map": f"{leaf_signal} is a port (no logic in this module)"
                        })

    return history, None

    # for key in binddict.keys():
    #     if module in str(key):
    #         print(f"Found key: {key}")

    # for key in binddict.keys():
    #     if module in str(key):
    #         if "immucr" in str(key):
    #             print(f"Found key: {key}")
    ########################################################################################


def find_signal_key(AST, given_signal, module):
    pattern = r'\(\s*\d+(?:\s*:\s*\d+)?\s*\)|\[\s*\d+(?:\s*:\s*\d+)?\s*\]'
    target_signal = re.sub(pattern, "", given_signal).split('.')[-1]  # Remove digits and brackets
    # print(f"Searching for signal: {target_signal}")
    for key in AST.keys():
        if target_signal == str(key).split('.')[-1] and str(key).split('.')[1] == module:
            # print(f"Matched: {str(key)}")
            return key
    return None
def hier_logic_trace_generation(AST, hier_logic_trace: str, results_dict: dict, module:str):
    if os.path.exists(f"{output_dir}{hier_logic_trace}"):
        print(f"File {hier_logic_trace} already exists. Skipping JSON generation.")
    else:
        nets_to_remove = []
        full_output = []

        # above line is equivalent to:
        nets = results_dict.keys()
        toggled_nets = [net for net in results_dict.keys() if results_dict[net]["toggles"] != 0]
        #
        net_matched = 0
        for net in nets:
            key = find_signal_key(AST, net, module)
            if key is None:
                print(f"Net {net} is not in the AST!")
            else:
                net_matched = net_matched + 1

        print(f"Total nets found in AST: {net_matched} out of {len(nets)}")

        with open(f"{output_dir}{hier_logic_trace}", "w") as f:
            # Signal to trace
            ID = 1  # Starting ID for JSON objects
            for signal_to_trace in nets:
                scoped_key = find_signal_key(AST, signal_to_trace, module)
                if scoped_key is None:
                    print(f"Net {signal_to_trace} is not in the AST!")
                    nets_to_remove.append(signal_to_trace)
                else:
                    tree = AST[scoped_key][0].tree
                    top_module = str(scoped_key).split('.')[-2]
                    history = [{
                        "module": top_module,
                        "assign": f"{signal_to_trace} = {tree.tocode()}"
                    }]
                    trace_history, final_expr = recursive_trace_json(AST, tree, max_depth=10, visited=set(),
                                                                     history=history)
                    full_output.append({
                        "id": ID,
                        "signal": signal_to_trace,
                        "module": top_module,
                        "trace_path": trace_history # it adds as a whole
                    })
                ID += 1
            json.dump(full_output, f, indent=2)
            print(f"JSON written to {hier_logic_trace}")



###################################################################################
############## Step 6: Generating Event Name and summary   ########################
###################################################################################

# temporary function to get module hierarchy
def get_module_hierarchy(net_transition_count_file, net_name):
    with open(net_transition_count_file, "r") as f:
        lines = f.readlines()
        for i, line in enumerate(lines):
            parts = line.strip().split()[0]
            if net_name.split(".")[-1] == parts.split(".")[-1]:  # Check if the net name matches
                net_parts = parts.strip().split(".")
                module = ".".join(net_parts[:-1])
                hier = module.replace(".", "->")
                return hier

def regeneration_architectural_events(results_dict, net_transition_count_file, signal_probability_csv, start_id,
                                      end_id):
    for key, data in results_dict.items():
        net_id = data.get("id")
        if net_id is None:
            continue
        if start_id <= net_id <= end_id:
            false_positive = data.get("toggles") == 0
            if not false_positive:
                if isinstance(data.get("llama_response"), dict) and data["llama_response"].get("high_level_event"):
                    print(f"Skipping {key} (already has LLM data)")
                    continue
                else:
                    sleep(2)
                    hier = get_module_hierarchy(net_transition_count_file, key)
                    result, token = semantic_search(key,hier,groq_chat,f"{output_dir}{hier_logic_trace}", module)
                    if result:
                        results_dict[key]["llama_response"] = result
                        results_dict[key]["usage_metadata"] = token
                        print(f"Updated net {key} (ID: {net_id}) with LLM response")
                    else:
                        print(f"No semantic search results found for net: {key}")
                        results_dict[key]["llama_response"] = None
                        results_dict[key]["usage_metadata"] = None
        with open(signal_probability_csv.replace("csv", "json"), "w") as f:
            json.dump(results_dict, f, indent=2)
    # with open(signal_probability_csv.replace("csv","json"), "w") as f:
    #     json.dump(results_dict, f, indent=2)

def update_architectural_events_with_previous_log(results_dict, log_file):
    pattern = r'\(\s*\d+(?:\s*:\s*\d+)?\s*\)|\[\s*\d+(?:\s*:\s*\d+)?\s*\]'
    # Regex patterns
    net_pattern = re.compile(r"Net:\s+(.+)")
    high_level_pattern = re.compile(r"High-Level Event:\s+(.+)")
    logical_pattern = re.compile(r"Logical Summary & Reasoning:\s+(.+?)(?=\nTest-Stimulus Guidance:)", re.S)
    stimulus_pattern = re.compile(r"Test-Stimulus Guidance:\s+(.+?)(?=\nInstruction Categories:)", re.S)
    instr_pattern = re.compile(r"Instruction Categories:\s*(.+)", re.S)
    with open(log_file, "r") as f:
        lines = f.readlines()

    # Process line by line
    current_block = []
    for line in lines:
        if net_pattern.match(line) and current_block:
            # Process previous block
            block_text = "".join(current_block)
            net_match = net_pattern.search(block_text)
            if net_match:
                net_name = net_match.group(1).strip()
                high_level = high_level_pattern.search(block_text)
                logical = logical_pattern.search(block_text)
                stimulus = stimulus_pattern.search(block_text)
                instr = instr_pattern.search(block_text)
                for dict_net_name in results_dict.keys():
                    target_signal = re.sub(pattern, "", dict_net_name).split('.')[-1]
                    if net_name == target_signal:
                        if "llama_response" in results_dict[dict_net_name]:
                            print(f"Skipping {dict_net_name} (already has LLM data)")
                            continue
                        else:
                            results_dict[dict_net_name]["llama_response"] = {
                                "net": dict_net_name,
                                "high_level_event": high_level.group(1).strip() if high_level else None,
                                "logical_summary_reasoning": logical.group(1).strip() if logical else None,
                                "test_stimulus_guidance": stimulus.group(1).strip() if stimulus else None,
                                "instruction_categories": instr.group(1).strip() if instr else None
                            }
                            print(f"Updated net: {dict_net_name}")

            # Start new block
            current_block = [line]
        else:
            current_block.append(line)

    # Handle last block if any
    if current_block:
        block_text = "".join(current_block)
        net_match = net_pattern.search(block_text)
        if net_match:
            net_name = net_match.group(1).strip()
            high_level = high_level_pattern.search(block_text)
            logical = logical_pattern.search(block_text)
            stimulus = stimulus_pattern.search(block_text)
            instr = instr_pattern.search(block_text)

            for dict_net_name in results_dict.keys():
                target_signal = re.sub(pattern, "", dict_net_name).split('.')[-1]
                if net_name == target_signal:
                    if "llama_response" in results_dict[dict_net_name]:
                        print(f"Skipping {dict_net_name} (already has LLM data)")
                        continue
                    else:
                        results_dict[dict_net_name]["llama_response"] = {
                            "net": dict_net_name,
                            "high_level_event": high_level.group(1).strip() if high_level else None,
                            "logical_summary_reasoning": logical.group(1).strip() if logical else None,
                            "test_stimulus_guidance": stimulus.group(1).strip() if stimulus else None,
                            "instruction_categories": instr.group(1).strip() if instr else None
                        }
                        print(f"Updated net: {dict_net_name}")

    with open(signal_probability_csv.replace("csv", "json"), "w") as f:
        json.dump(results_dict, f, indent=2)


if __name__ == "__main__":
    def main():
        pass
    from semantic_search_with_history import *
    from llm_Setup import *
    stage = "ctrl"
    cpu = "mor1kx"
    version = "cappuccino"
    module = cpu + "_" + stage + "_" + version
    # module = cpu + "_" + stage
    hier_logic_trace = "hierarchical_logic_trace_" + module + ".json"
    vcd_text_file_dir = "/home/m588h354/projects/autophasew/openrisc/src/vcd_texts/"
    vcd_text = vcd_text_file_dir + stage + "3_aes.txt"  # single text file of vcd to analyze
    output_dir = "/home/m588h354/projects/autophasew/openrisc/src/architectural_events/nets_directory/"
    all_net_transitions_count = (output_dir + module + "_all_transitions.txt")
    vcd_texts_cappuccino = stage + "3_*.txt"  # generated from running bencmarks
    vcd_texts_marocchino = "u_" + stage + "3_*.txt"  # generated from running bencmarks
    signal_probability_csv = (output_dir + module + "_p_hat_tau.csv")

    # ------------------------
    step = 5
    match step:
        case 1:
            ###################################################################################
            ############## Step 1: Collect all the nets transitions from VCD text file ########
            # Input: vcd text files
            # Output: A single file with all the nets and their transition counts
            # Currently not using in our framework but can be observed for understanding
            ###################################################################################
            if not os.path.exists(all_net_transitions_count):
                collect_all_transitions_from_vcd_text(vcd_text_file_dir, vcd_texts_cappuccino,
                                                      all_net_transitions_count)
            else:
                print(f"Output file '{all_net_transitions_count}' already exists. Skipping collection of transitions.")
        case 2:
            #step 2: Compute signal probabilities and toggles from the VCD text file
            ###################################################################################
            ############## Step 2: Caculate signal probability from the vcd text file##########
            # Input: single or multiple vcd text files
            # Output: CSV file with p_hat_1, p_hat_0, R_v_min, toggles, steps, tau, is_bus
            ###################################################################################

            if not os.path.exists(signal_probability_csv):
                with open(all_net_transitions_count) as f:
                    for line in f:
                        if module in line:
                            anchor = line.split(module, 1)[0] + module
                            break
                f.close()
                if not anchor:
                    raise ValueError(f"Anchor for module '{module}' not found in {all_net_transitions_count}.")
                # compute_signal_probabilities_single_file(vcd_text, anchor, signal_probability_csv)
                compute_signal_probabilities_multiple_file(vcd_text_file_dir+vcd_texts_cappuccino, anchor,
                                                           signal_probability_csv)
            else:
                print(f"Output file '{signal_probability_csv}' already exists. Skipping computation.")
        case 3:
            ###################################################################################
            ############## Step 3: Choose Rare nets based on theta and  #######################
            # ############ grouping them and create a new dict from existing CSV #############
            ###################################################################################
            results_dict = rare_net_selection(signal_probability_csv, plot=True)
        case 4:

            results_dict = rare_net_selection(signal_probability_csv, plot=False)
            AST = verilog2AST()
            # for key in AST.keys():
            #     if module in str(key):
            #         print(f"key: {key}")
            #         if "dc_sdat" in str(key):
            #             print(f"Found key: {key}")

            hier_logic_trace_generation(AST, hier_logic_trace, results_dict, module)
            # --- Save initial dictionary ---
            if not os.path.exists(signal_probability_csv.replace("csv", "json")):
                with open(signal_probability_csv.replace("csv", "json"), "w") as f:
                    json.dump(results_dict, f, indent=4)
        case 5:
            ###################################################################################
            ############## Step 5: Generating Event Name and summary   ########################
            ###################################################################################
            with open(signal_probability_csv.replace("csv", "json"), "r") as f:
                results_dict = json.load(f)
            start_id = 0
            end_id = 1000
            regeneration_architectural_events(results_dict, all_net_transitions_count, signal_probability_csv,
                                              start_id, end_id)
            # log_file = ("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/"
            #             "event_files/HIGH_LEVEL_EVENTS_CTRL_107_176.txt")
            # update_architectural_events_with_previous_log(results_dict, log_file)
        case 6:
            mch = find_fuzzy_trace_path(output_dir + hier_logic_trace,"dc_adr", module)
            print(mch)

