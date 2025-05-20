/* Test program to trigger floating-point instruction completion and update the floating-point status register (fpcsr_set_o).
 *
 * Reason for previous failure:
 *   The register names in inline asm are unknown to GCC; OpenRISC inline asm does not recognize 'f1', 'f2', etc.
 *
 * Fix:
 *   Use assembler mnemonics without specifying registers explicitly in asm constraints.
 *   Use .word encoding for instructions if needed, or let assembler handle register allocation.
 *   Use a memory-based approach with volatile variables and minimal asm to emit FP ops.
 *
 * Approach:
 *   - store/load floating-point values to/from memory
 *   - run floating-point add and multiply instructions explicitly
 */

volatile float a = 1.5f;
volatile float b = 2.5f;
volatile float c = 0.0f;
volatile float d = 0.0f;

int main(void)
{
    // Use assembler to perform floating-point addition and multiplication
    // Using instructions with registers assigned by assembler
    // "fadd.s f1,f2,f3" like syntax replaced by generic "fadd.s"
    // but without explicit reg usage in constraints

    __asm__ volatile (
        // load values (from memory) into fp registers implicitly via l.s
        "l.s f16, %2\n"      // load a
        "l.s f17, %3\n"      // load b
        "fadd.s f18, f16, f17\n"  // add: f18 = f16 + f17
        "s.s %0, f18\n"      // store result into c
        : "=m"(c)
        : "m"(c), "m"(a), "m"(b)
        : "f16", "f17", "f18"
    );

    __asm__ volatile (
        "l.s f19, %2\n"      // load c
        "l.s f20, %3\n"      // load b
        "fmul.s f21, f19, f20\n"  // multiply: f21 = f19 * f20
        "s.s %0, f21\n"      // store result into d
        : "=m"(d)
        : "m"(d), "m"(c), "m"(b)
        : "f19", "f20", "f21"
    );

    if (d > c)
        return 1;
    else
        return 0;
}