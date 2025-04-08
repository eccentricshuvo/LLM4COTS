from dotenv import load_dotenv
from mypy.state import state

load_dotenv()
from openAI_assistant import generate_response
from langchain_groq import ChatGroq
from groq import Groq
from langchain.llms.base import LLM

#%%
class ChatGroqLLM(LLM):
    def __init__(self, groq_api_key, model_name):
        client = Groq(
            api_key=groq_api_key,  # Replace with your Groq API key
        )

    def _call(self, prompt: str, stop=None) -> str:
        """
        Call the underlying ChatGroq LLM with the given prompt and return the response.
        """
        chat_completion = client.chat.completions.create(
            messages=[
                {
                    "role": "user",
                    "content": prompt,
                }
            ],
            model=model_name,
        )
        # Use the generate method with prompt directly
        response = chat_completion.choices[0].message.content

        return response

    @property
    def _llm_type(self) -> str:
        return "chat_groq"


groq_api_key = "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"
# Initialize Groq Langchain chat object and conversation
groq_chat = ChatGroq(
    groq_api_key=groq_api_key,
    model_name="llama-3.3-70b-versatile"
)


#%%
from typing import TypedDict, Annotated, List, Sequence
from langchain_core.messages import BaseMessage, HumanMessage, AIMessage, SystemMessage
from langchain.schema import Document
from pydantic import BaseModel, Field
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.prompts import ChatPromptTemplate
from langgraph.graph import StateGraph, END
from langgraph.graph import add_messages
from typing import Literal


class AgentState(TypedDict):
    messages: List[BaseMessage]
    # messages: Annotated[Sequence[BaseMessage], add_messages]
    documents: List[Document]
    summary: str
    on_topic: str
    std_or_inline: str
    rephrased_question: str
    proceed_to_generate: bool
    re_retrieve_count: int
    question: HumanMessage
    event_is_triggered: str
    output_program: str
    feedback: str
    re_generate_count: int

from langgraph.checkpoint.memory import MemorySaver
checkpointer = MemorySaver()

def question_rewriter(state: AgentState) -> StateGraph:
    print(f"Entering question_rewriter with following state: {state}")
    state["documents"] = []
    state["on_topic"] = ""
    state["std_or_inline"] = ""
    state["rephrased_question"] = ""
    state["proceed_to_generate"] = False
    state["re_retrieve_count"] = 0
    state["event_is_triggered"] = "NA"
    state["output_program"] = ""
    state["feedback"] = ""
    state["re_generate_count"] = 0

    # But we don't want to reset the messages these are responsible for the conversation history
    if "messages" not in state or state["messages"] is None:
        state["messages"] = []
    if state["question"] not in state["messages"]:
        state["messages"].append(state["question"])

    # if len(state["messages"]) > 1:
    #     conversation = state["messages"][:-1]
    #     print(f"question_rewriter: Conversation: {conversation}")
    #     current_question = state["question"].content
    #     print(f"question_rewriter: Current question: {current_question}")
    #     messages = [
    #         SystemMessage(content="You are a helpful assistant that rephrases the users's question to be a standalone question optimized for retrieval.")
    #
    #     ]
    #     messages.extend(conversation) # Don't know why?
    #
    #     messages.append(HumanMessage(content=current_question))
    #     print(f"Messages==========: {messages}")
    #     rephrase_prompt = ChatPromptTemplate.from_messages(messages)
    #     llm = ChatOpenAI(model="gpt-3.5-turbo")
    #     prompt = rephrase_prompt.format()
    #     response = llm.invoke(prompt)
    #     better_question = response.content.strip()
    #     print(f"question_rewriter: Rephrased question: {better_question}")
    #     state["rephrased_question"] = better_question
    # else:
    #     state["rephrased_question"] = state["question"].content
    state["rephrased_question"] = state["question"].content
    return state


# Topic Classifier
class GradeTopic(BaseModel):
    #score: str = Field(description="Question is about the specified topics? If 1 or 2 or 3 -> 'Yes' else 'No'")
    score: Literal["Yes", "No"] = Field(description="Does the user input match any of the specified topics?")


def topic_classifier(state: AgentState):
    print(f"Entering topic_classifier with following state: {state}")
    system_message = SystemMessage(
        content="""You are an expert in computer architecture. You will be given a user's input (question or phrase). Your job is to determine whether the input matches **any** of the following types:

- An architectural event or its description from RISC microprocessor architecture (e.g., LSU activation, memory access initiation, detection of a page fault exception, cache invalidation request)
- A signal or net name from hardware description language (HDL) that implies an architectural event
- A snippet of HDL code

If it matches **any** of the above categories, respond only with 'Yes'. If it does not match **any**, respond only with 'No'. Do not output anything else."""
    )

    human_message = HumanMessage(
        # content=f"User's question: {state['question'].content}"
        content = f"User's question: {state['rephrased_question']}"
    )

    grade_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    llm = ChatOpenAI(model="gpt-3.5-turbo")
    # llm = groq_chat
    structured_llm = llm.with_structured_output(GradeTopic)
    grader_llm = grade_prompt | structured_llm
    result = grader_llm.invoke({})
    print(f"topic_classifier: Result: {result}")
    state["on_topic"] = result.score.strip()
    print(f"topic_classifier: On topic: {state['on_topic']}")
    return state

# Event Classifier
class GradeEvent(BaseModel):
    score: Literal["Standard", "Inline"] = Field(
        description="Does the event require inline assembly to be triggered?"
    )

def event_classifier(state: AgentState):
    print(f"Entering event_classifier with following state: {state}")
    system_message = SystemMessage(
    content="""You are an expert in computer architecture and low-level embedded programming. You will be given a 
    user's input describing an architectural event related to OpenRISC 1000 processor.

Your task is to classify whether the event **can be triggered using only standard C code**, or if it **requires 
inline assembly inside C code**.

Use these rules:
- If the event can be triggered using common C constructs such as loads, stores, arithmetic, conditionals, or function calls, then respond with **'Standard'**
- If the event requires special-purpose registers (SPRs), privileged instructions, atomic operations, MMU/TLB setup, illegal opcodes, or instructions that are not guaranteed to be generated by a C compiler, then respond with **'Inline'**

**Respond with only one word: 'Standard' or 'Inline'. Do not explain.**
"""
)
    human_message = HumanMessage(
        # content=f"User's question: {state['question'].content}"
        content=f"User's question: {state['rephrased_question']}"
    )

    grade_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    llm = ChatOpenAI(model="gpt-3.5-turbo")
    # llm = groq_chat
    structured_llm = llm.with_structured_output(GradeEvent)
    grader_llm = grade_prompt | structured_llm
    result = grader_llm.invoke({})
    print(f"event_classifier: Result: {result}")
    state["std_or_inline"] = result.score.strip()
    print(f"event_classifier: Standard or Inline: {state['std_or_inline']}")
    return state

def on_topic_router(state: AgentState):
    print(f"Entering on_topic_router with following state: {state}")
    on_topic = state["on_topic"].lower()
    if on_topic == "yes":
        print("Routing to Event Classifier")
        return "event_classifier"
    else:
        print("Routing to off_topic response")
        return "off_topic_response"

def std_or_inline_router(state: AgentState):
    print(f"Entering std_or_inline_router with following state: {state}")
    on_topic = state["std_or_inline"].lower()
    if on_topic == "standard":
        print("Routing to Standard Retriever")
        return "std_retrieve"
    else:
        print("Routing to Inline Retriever")
        return "inline_retrieve"

def off_topic_response(state: AgentState):
    print("Entering off_topic_response")
    if "messages" not in state or state["messages"] is None:
        state["messages"] = []
    state["messages"].append(
        AIMessage(
            content="I'm sorry, I can only answer questions about the OpenRISC 1000 high level events"
        )
    )
    return state
def couldnot_generate(state: AgentState):
    print("Entering couldnot_generate")
    if "messages" not in state or state["messages"] is None:
        state["messages"] = []
    state["messages"].append(
        AIMessage(
            content="I'm sorry, I couldn't generate a program for the given event"
        )
    )
    return state
#%%
# ######################### Retriever ############################
# from langchain.schema import Document
# from langchain.storage import InMemoryByteStore
# from langchain_chroma import Chroma
#
# from langchain_openai import OpenAIEmbeddings, ChatOpenAI
# # from langchain_community.vectorstores import Chroma
# from langchain_community.document_loaders import TextLoader
#
# embedding_fn = OpenAIEmbeddings()
#
# from langchain.document_loaders import PyPDFLoader
# from langchain.text_splitter import RecursiveCharacterTextSplitter
# pages = []
# manuals = [
#     PyPDFLoader("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/openrisc-arch-1.4-rev0.pdf"),
#     # TextLoader("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/or1k-sprs.h"),
#     # TextLoader("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/or1k-support.h"),
#     # TextLoader("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/GOLDEN_HIGH_LEVEL_EVENTS.txt"),
#     ]
# for manual in manuals:
#     pages.extend(manual.load())
#
# splitter = RecursiveCharacterTextSplitter(
#     # chunk_size=800, chunk_overlap=100, separators=["\n\n", "\n", ".", " "]
#     chunk_size=10000
# )
#
# docs = splitter.split_documents(pages)
# # The vectorstore to use to index the child chunks
# vectorstore = Chroma(
#     collection_name="full_documents", embedding_function=OpenAIEmbeddings()
# )
# # db = Chroma.from_documents(docs, embedding_fn)
# # retriever = db.as_retriever(search_kwargs={"k": 2})
# import uuid
#
# from langchain.retrievers.multi_vector import MultiVectorRetriever
# # The storage layer for the parent documents
# store = InMemoryByteStore()
# id_key = "doc_id"
#
# # The retriever (empty to start)
# retriever = MultiVectorRetriever(
#     vectorstore=vectorstore,
#     byte_store=store,
#     id_key=id_key,
# )
#
# doc_ids = [str(uuid.uuid4()) for _ in docs]
# child_text_splitter = RecursiveCharacterTextSplitter(
#     chunk_size=800, chunk_overlap=100, separators=["\n\n", "\n", ".", " "]
#     #chunk_size=400
# )
# sub_docs = []
# for i, doc in enumerate(docs):
#     _id = doc_ids[i]
#     _sub_docs = child_text_splitter.split_documents([doc])
#     for _doc in _sub_docs:
#         _doc.metadata[id_key] = _id
#     sub_docs.extend(_sub_docs)
#
#
# retriever.vectorstore.add_documents(sub_docs)
# retriever.docstore.mset(list(zip(doc_ids, docs)))
#%%
# from langchain.retrievers.multi_vector import SearchType
# retriever.search_type = SearchType.mmr
#
# def get_retriever_prompt(event_description: str) -> str:
#     return f"""
# You're preparing to generate a test program for an architectural event: "{event_description}".
#
# Your primary goal is to use minimal inline assembly to trigger this event.
# Retrieve only the information from architecture manuals that is relevant to use inline assembly to trigger this
# event. Your response will pass to the test program generator to create a test program that intentionally triggers this event.
# """
# # - What instructions from ISA is relevant for inline assembly?
# # - What causes this event?
# # - Which SPRs or privilege levels are relevant (if any)?
# # - Does this event require special setup (e.g., MMU enabled, exceptions enabled)?
# #
# # Do not include general background unless it directly relates to triggering this event.
# # """
# query = get_retriever_prompt("signal pipeline_flush_i")
# input = "What causes this event : set flag if equal?"
# #The vector store alone will retrieve small chunks:
# retriever.vectorstore.similarity_search(input, k = 3)
#
# #Whereas the retriever will return the larger parent document:
# #len(retriever.invoke("justice breyer")[0].page_content)

######################### Retriever END ############################
#%%
######################### OPENAI File Assistant  ####################
from openai import OpenAI
client = OpenAI(
    api_key = "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",
    organization= None
)
#%%
# # --------------------------------------------------------------
# # Create assistant
# # --------------------------------------------------------------
# assistant = client.beta.assistants.create(
#     name="decode_stage_event_reference",
#     instructions="""
#     This file contains detailed descriptions of output signals from the decode stage of the OpenRISC 1000 microprocessor.
#   Each entry includes:
#     - The signal name (Net)
#     - The high-level architectural event it represents
#     - A summary of the logical conditions that cause the signal to activate
#     - A reasoning section explaining its role and purpose in the processor pipeline
#
#   You should:
#     - Use the `Net:` field to search for specific signal names
#     - Use `High-Level Event:` to understand what processor behavior is associated with a signal
#     - When asked about an event, return the "logical summary" and "reasoning" of the event
#     """,
#     model="gpt-3.5-turbo",
#     tools=[{"type": "file_search"}]
# )
# vector_store = client.beta.vector_stores.create(name="decode_stage_event_reference")
# file_paths = ["/home/m588h354/projects/Rare_net_analysis-repo/event_identification/event_files/HIGH_LEVEL_EVENTS_DECODE.txt"]
# file_streams = [open(path, "rb") for path in file_paths]
#
# # Use the upload and poll SDK helper to upload the files, add them to the vector store,
# # and poll the status of the file batch for completion.
# file_batch = client.beta.vector_stores.file_batches.upload_and_poll(
#   vector_store_id=vector_store.id, files=file_streams
# )
# print(file_batch.status)
# print(file_batch.file_counts)
#%%
# assistant = client.beta.assistants.update(
#     assistant_id=assistant.id,
#     model="gpt-3.5-turbo",
#     tool_resources={"file_search": {"vector_store_ids": [vector_store.id]}},
# )
# assistant.id
#%%
# response = generate_response(assistant, "Return me logical summary and reasoning of high level event: decode_except_illegal_o","5", "test1")
# response
#%%
decode_events_id = "asst_yBrlPq2dAcuSa1er6Ghxcfep"
openRISC_manual_id = "asst_UoYBmjJNgNNznOfJRoZMGPDx"
assistant_inline = client.beta.assistants.retrieve(openRISC_manual_id)
assistant = client.beta.assistants.retrieve(decode_events_id)
# response = generate_response(assistant_inline, "ISA for event set flag if equal","3", "test2")
# response
#%%
def std_retrieve(state: AgentState) -> StateGraph:
    print(f"Entering std_retrieve with following state: {state}")
    # documents = retriever.vectorstore.similarity_search(state["rephrased_question"])
    event = state["rephrased_question"]
    documents = (generate_response(assistant, f"Return me logical summary and reasoning of high level event: "
                                              f"{event}", "10", "test"))
    print(f"std_retrieve: Retrieved documents: {documents}")
    # print("==================")
    # print(f"retrieve: Retrieved {len(documents)} documents")
    state["documents"] = documents
    return state

def inline_retrieve(state: AgentState) -> StateGraph:
    print(f"Entering inline_retrieve with following state: {state}")
    # documents = retriever.vectorstore.similarity_search(state["rephrased_question"])
    event = state["rephrased_question"]
    documents = (generate_response(assistant, f"Return me logical summary and reasoning of high level event: "
                                              f"{event}", "12", "test"))
    print(f"inline_retrieve: Retrieved documents: {documents}")
    # print("==================")
    # print(f"retrieve: Retrieved {len(documents)} documents")
    state["documents"] = documents
    return state

class GradeDocument(BaseModel):
    score: str = Field(
        description="Document is relevant to the question? If yes -> 'Yes' if not -> 'No'"
    )

def retrieval_grader(state: AgentState):
    print("Entering retrieval_grader")
    system_message = SystemMessage(
        content="""You are a grader assessing the relevance of a retrieved document to a user question. Only answer 
        with 'Yes' or 'No'.
        If the document contains information relevant to the user's question, respond with 'Yes'. Otherwise, 
        respond with 'No'."""
    )
    llm = ChatOpenAI(model="gpt-3.5-turbo")
    structured_llm = llm.with_structured_output(GradeDocument)
    relevant_docs = []
    # for doc in state["documents"]:
    #     human_message = HumanMessage(
    #         content=f"User question: {state['rephrased_question']}\n\nRetrieved document:\n{doc.page_content}"
    #     )
    #     grade_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    #     grader_llm = grade_prompt | structured_llm
    #     result = grader_llm.invoke({})
    #     print(f"Grading document: {doc.page_content[:30]}... Result: {result.score.strip()}")
    #     if result.score.strip().lower() == "yes":
    #         relevant_docs.append(doc)
    doc = state["documents"]
    human_message = HumanMessage(
            content=f"User question: {state['rephrased_question']}\n\nRetrieved document:\n{doc}"
        )
    grade_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    grader_llm = grade_prompt | structured_llm
    result = grader_llm.invoke({})
    print(f"Grading document: {doc[:30]}... Result: {result.score.strip()}")
    if result.score.strip().lower() == "yes":
        relevant_docs.append(doc)
    else:
        relevant_docs.append(doc)

    state["documents"] = relevant_docs
    state["proceed_to_generate"] = len(relevant_docs) > 0
    print(f"retrieval_grader: Proceed to generate: {state['proceed_to_generate']}")
    return state

def proceed_retriever(state: AgentState):
    print(f"Entering proceed_retriever with following state: {state}")
    re_retrieve_count = state.get("re_retrieve_count", 0)
    if state.get("proceed_to_generate", False):
        print("Routing to program_generator")
        return "program_generator"
    elif re_retrieve_count >= 2:
        print("Maximal rephrasing attempts reached. Cannot find relevant documents...")
        return "couldnot_generate"
    else:
        print("Routing to refine event details")
        return "refine_event_details"

def refine_event_details(state: AgentState):
    print(f"Entering refine_event_details with following state: {state}")
    re_retrieve_count = state.get("re_retrieve_count", 0)
    if re_retrieve_count >= 2:
        print("Maximal re retrieving attempts reached. Cannot find relevant documents...")
        return state
    event_to_refine = state["rephrased_question"]
    system_message = SystemMessage(
        content="""You are a helpful assistant that slightly refines the user's question to improve retrieval results.
    Provide a slightly adjusted version of the question."""
    )
    human_message = HumanMessage(
        content=f"Original question: {event_to_refine}\n\nProvide a slightly refined question."
    )
    refine_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    llm = ChatOpenAI(model="gpt-3.5-turbo")
    prompt = refine_prompt.format()
    response = llm.invoke(prompt)
    refined_event = response.content.strip()
    print(f"refine_question: Refined question: {refined_event}")
    state["rephrased_question"] = refined_event
    state["re_retrieve_count"] = re_retrieve_count + 1
    return state

from langchain_core.prompts import ChatPromptTemplate

template = """You are an expert in computer architecture. I will provide a high-level architectural event name along 
with a description. Your task is to write a complete C test program that triggers this event in the microprocessor.
Follow these strict rules:
- No inline assembly code inside the C code
- Complete C code with instructions. No suggesstions or hints or Do something type comments.
- Do not include any printf, puts, or output statements.
Feedback: {feedback}
History: {history}
Architectural Event: {question}
Description: {context}
Please only return the complete C code that triggers the event. Do not include any additional information or comments.
"""
prompt = ChatPromptTemplate.from_template(template)

# rag_chain = prompt | groq_chat
rag_chain = prompt | ChatOpenAI(model="gpt-3.5-turbo")

def program_generator(state: AgentState):
    print("Entering program_generator")
    # history = state["messages"]
    documents = state["documents"]
    rephrased_question = state["rephrased_question"]
    if state["event_is_triggered"].lower() == "yes":
        return state
    elif state["event_is_triggered"].lower() == "NA":
        feedback = "Not Applicable"
        history = "Not Applicable"
    else:
        feedback = state["feedback"]
        history = state["output_program"]
        state["re_generate_count"] += 1
    print(f"program_generator: Feedback: {feedback}")
    print(f"program_generator: History: {history}")

    response = rag_chain.invoke(
        {
            "history": history,
            "feedback": feedback,
            "context": documents,
            "question": rephrased_question
        }
    )
    print(f"program_generator: Generated program: {response.content}")
    state["output_program"] = response.content.strip()

    return state

def find_net_for_event(file_path, target_event):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    result = None
    for i in range(1, len(lines)):
        line = lines[i].strip()
        if line.startswith("- High-Level Event:") and target_event in line:
            prev_line = lines[i - 1].strip()
            # print(f"Previous line: {prev_line}")
            if prev_line.startswith("- Net:"):
                result = prev_line.replace("- Net: ", "").strip()
                break
    return result

def event_triggered(state: AgentState):
    print("Entering event_triggered")
    program_code = state["output_program"]

    # Remove the Markdown code block markers if they exist
    if program_code.startswith("```c") and program_code.endswith("```"):
        program_code = program_code.strip("```c").strip("```").strip()
    # create a file name from the question
    program_file = state["rephrased_question"].replace(" ", "_").replace(":", "").replace("(", "").replace(")", "")
    # Save to a file
    arch_event_dir = "/home/m588h354/projects/autophasew/openrisc/src/architectural_events/decode_events"
    with open(
            f"{arch_event_dir}/{program_file}.c",
            "w") as file:
        file.write(program_code)

    import subprocess
    project_root = "/home/m588h354/projects/autophasew/openrisc/src"
    c_file = f"{arch_event_dir}/{program_file}.c"
    output_file = f"{arch_event_dir}/{program_file}.o"
    vcd_file_name = "decode3.vcd"
    vcd_file = "vcd_files/decode3.vcd"
    vcd_txt = "vcd_texts/decode3.txt"
    vcd_text_org = "vcd_texts/decode.txt"
    event_details_file = "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/event_files/HIGH_LEVEL_EVENTS_DECODE.txt"
    net_name  = find_net_for_event(event_details_file, state["rephrased_question"])
    print(f"Net name for event '{state['rephrased_question']}': {net_name}")
    gtkw_file = "gtkw_files/decode.gtkw"

    # Step 1: Compile with or1k-elf-gcc
    try:
        subprocess.run(
            ["/home/m588h354/projects/autophasew/openrisc/or1k-elf/bin/or1k-elf-gcc", c_file, "-o", output_file],
            check=True
        )
        print("Compilation successful.")
    except subprocess.CalledProcessError as e:
        print("Compilation failed.")
        print(e)
        exit(1)

    # Step 2: Run simulation with fusesoc
    try:
        subprocess.run([
            "fusesoc", "run", "--target", "mor1kx_tb", "--tool", "verilator",
            "mor1kx-generic", "--elf_load", output_file,
            "--vcd", "testbench.vcd",
        ], check=True, cwd=project_root, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        print("Simulation completed.")
    except subprocess.CalledProcessError as e:
        print("Simulation failed.")
        print(e)
        exit(1)

    # Step 3: Launch GTKWave using export script
    try:
        subprocess.run([
            "./export_vcd_gtkwave.sh", gtkw_file, vcd_file_name
        ], check=True, cwd=project_root)
        print("GTKWave launched with specified VCD and GTKWave file.")
    except subprocess.CalledProcessError as e:
        print("GTKWave launch failed.")
        print(e)

    import os
    # Step 4: vcdcat
    try:
        with open(os.path.join(project_root, vcd_txt), "w") as txt_file:
            subprocess.run(["vcdcat", "-d", vcd_file], cwd=project_root, stdout=txt_file, check=True)
        print("VCD dumped to text.")
    except subprocess.CalledProcessError as e:
        print("vcdcat failed.")
        print(e)

    # Step 5: grep + awk + wc -l to count non-zero activity
    try:
        result_post = subprocess.run(
            f"grep -E '{net_name}' {vcd_txt} | awk '$2 != 0' | wc -l",
            shell=True,
            cwd=project_root,
            capture_output=True,
            text=True,
            check=True
        )
        result_pre = subprocess.run(
            f"grep -E '{net_name}' {vcd_text_org} | awk '$2 != 0' | wc -l",
            shell=True,
            cwd=project_root,
            capture_output=True,
            text=True,
            check=True
        )
        print(f"Number of non-zero transitions(post) for '{net_name}': {result_post.stdout.strip()}")
        print(f"Number of non-zero transitions(pre)  for '{net_name}': {result_pre.stdout.strip()}")
        count_post = int(result_post.stdout.strip())
        count_pre = int(result_pre.stdout.strip())
        if count_post - count_pre > 0:
            print("Event triggered successfully!")
            state["event_is_triggered"] = "yes"
        else:
            print("Event did not trigger.")
            state["event_is_triggered"] = "no"
    except subprocess.CalledProcessError as e:
        print("grep/awk/wc command failed.")
        print(e)

    # state["event_is_triggered"] = "yes"
    return state
def trigger_router(state: AgentState):
    print(f"Entering trigger_route")
    event_trigger = state["event_is_triggered"].lower()
    if event_trigger == "yes":
        print("Event Triggered :D")
        return "event_trigger_success"
    elif event_trigger == "no" and state["re_generate_count"] > 2:
        print("Maximal re generation attempts reached. Cannot generate a program...")
        return "couldnot_generate"
    else:
        print("Routing to Add Compiled Log")
        state["feedback"] = """
        The previous generated program did not trigger the event. Please check the History where the previous program is stored. Please reconsider the event and anlyse it further why it didn't trigger last time. Based on that change the program to trigger this event.
        """
        return "add_compiled_log"

def add_compiled_log(state: AgentState):
    print("Entering add_compiled_log")
    return state


#%%
workflow = StateGraph(AgentState)
workflow.add_node("question_rewriter", question_rewriter)
workflow.add_node("topic_classifier", topic_classifier)
workflow.add_node("std_retrieve", std_retrieve)
workflow.add_node("inline_retrieve", inline_retrieve)
workflow.add_node("event_classifier", event_classifier)
workflow.add_node("retrieval_grader", retrieval_grader)
workflow.add_node("program_generator", program_generator)
workflow.add_node("event_triggered", event_triggered)
workflow.add_node("add_compiled_log", add_compiled_log)
workflow.add_node("refine_event_details", refine_event_details)
workflow.add_node("off_topic_response", off_topic_response)
workflow.add_node("couldnot_generate", couldnot_generate)

workflow.add_edge("question_rewriter", "topic_classifier")
workflow.add_conditional_edges(
    "topic_classifier",
    on_topic_router,
    {
        "off_topic_response": "off_topic_response",
        "event_classifier": "event_classifier"
    }
)
workflow.add_conditional_edges(
    "event_classifier",
    std_or_inline_router,
    {
        "std_retrieve": "std_retrieve",
        "inline_retrieve": "inline_retrieve"
    }
)
workflow.add_edge("std_retrieve", "retrieval_grader")

workflow.add_conditional_edges(
    "retrieval_grader",
    proceed_retriever,
    {
        "couldnot_generate": "couldnot_generate",
        "refine_event_details": "refine_event_details",
        "program_generator": "program_generator",

    }
)
workflow.add_edge("inline_retrieve", "program_generator")
workflow.add_edge("refine_event_details", "std_retrieve")
workflow.add_edge("program_generator", "event_triggered")
workflow.add_conditional_edges(
    "event_triggered",
    trigger_router,
    {
        "couldnot_generate": "couldnot_generate",
        "event_trigger_success": END,
        "add_compiled_log": "add_compiled_log",
    }
)
workflow.add_edge("add_compiled_log", "program_generator")
workflow.add_edge("off_topic_response", END)
workflow.add_edge("couldnot_generate", END)

workflow.set_entry_point("question_rewriter")
graph = workflow.compile(checkpointer=checkpointer)
#
# from langchain_core.runnables.graph import MermaidDrawMethod
# from PIL import Image
# img_bytes = graph.get_graph().draw_mermaid_png(
#     draw_method=MermaidDrawMethod.API,
# )
#
# # Save to a file
# with open("workflow_graph.png", "wb") as f:
#     f.write(img_bytes)
#
# # Optional: Display it using PIL (pillow)
# img = Image.open("workflow_graph.png")
# img.show()  # This will open the image using your OS default viewer

#%%
input_data = {"question": HumanMessage(content="Addition with carry instruction decoding")}
#
response = graph.invoke(input_data, config={"configurable": {"thread_id": 19}})
print("===================================================")

#%%
program_code = response.get("output_program")

# Remove the Markdown code block markers if they exist
if program_code.startswith("```c") and program_code.endswith("```"):
    program_code = program_code.strip("```c").strip("```").strip()

# Save to a file
with open("/home/m588h354/projects/autophasew/openrisc/src/architectural_events/decode_events/branch_condition_decode"
          ".c", "w") as file:
    file.write(program_code)

print("C program saved to branch_condition_decode.c")

#%%
