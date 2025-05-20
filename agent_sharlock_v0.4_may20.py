#!/usr/bin/env python
# coding: utf-8
import time

from accelerate.test_utils.scripts.test_script import test_trigger
from babel.dates import time_
# ##### Load the OpenAI API Key and Groq Chat Environment

# In[77]:

import random
from dotenv import load_dotenv
from mypy.state import state

from helper import count_transitions

load_dotenv()
from openAI_assistant import generate_response, delete_thread
from langchain_groq import ChatGroq
from groq import Groq
from langchain.llms.base import LLM

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


groq_api_key = ""
# Initialize Groq Langchain chat object and conversation
groq_chat = ChatGroq(
    groq_api_key=groq_api_key,
    model_name="llama-3.3-70b-versatile"
)


# ##### Define the directory path once

# In[78]:

or1k_compiler = "openrisc/or1k-elf/bin/or1k-elf-gcc"

project_root = "projects/autophasew/openrisc/src"
event_root = "projects/Rare_net_analysis-repo/event_identification"
stage = "lsu"
hle_file_name = "HIGH_LEVEL_EVENTS_LSU_0_20"
arch_event_dir = f"{project_root}/architectural_events/{stage}_events"
event_details_file = f"{event_root}/event_files/{hle_file_name}.txt"
json_file_retrieve = f"{event_root}/event_files/{hle_file_name}.json"


# ##### Create Class Agent State

# In[79]:


from typing_extensions import TypedDict, Annotated, List, Sequence, Optional
from langchain_core.messages import BaseMessage, HumanMessage, AIMessage, SystemMessage
from langchain.schema import Document
from pydantic import BaseModel, Field
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.prompts import ChatPromptTemplate
from langgraph.graph import StateGraph, END
from langgraph.graph import add_messages
from typing import Literal

class Transition(TypedDict):
    post_transition: int
    pre_transition: int
    post_cycles: int
    pre_cycles: int
class ProgramEntry(TypedDict):
    code: str
    compilation_error: Optional[str]
    event_triggered: Optional[Literal["yes", "no", ""]]
    timeout: Optional[Literal["yes", "no", ""]]
    transition: Optional[Transition] # Optional means it can be None

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
    output_program: List[ProgramEntry]
    feedback: str
    re_generate_count: int
    compilation_error: str
    thread_id: str
    thread_name: str
    llm_iteration: int

from langgraph.checkpoint.memory import MemorySaver
checkpointer = MemorySaver()


# In[ ]:





# ##### STATE question_rewriter

# In[80]:


def question_rewriter(state: AgentState) -> StateGraph:
    print(f"Entering question_rewriter")
    state["documents"] = []
    state["on_topic"] = ""
    state["std_or_inline"] = ""
    state["rephrased_question"] = ""
    state["proceed_to_generate"] = False
    state["re_retrieve_count"] = 0
    state["event_is_triggered"] = "no"
    state["output_program"] = []
    state["feedback"] = ""
    state["re_generate_count"] = 0
    state["compilation_error"] = ""
    state["thread_id"] = str(0)
    state["thread_name"] = ""
    state["llm_iteration"] = 3
    

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


# ##### STATE topic_classifier

# In[81]:


# Topic Classifier
class GradeTopic(BaseModel):
    #score: str = Field(description="Question is about the specified topics? If 1 or 2 or 3 -> 'Yes' else 'No'")
    score: Literal["Yes", "No"] = Field(description="Does the user input match any of the specified topics?")


def topic_classifier(state: AgentState):
    print(f"Entering topic_classifier")
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
    # llm = ChatOpenAI(model="gpt-3.5-turbo")
    llm = groq_chat
    structured_llm = llm.with_structured_output(GradeTopic)
    grader_llm = grade_prompt | structured_llm
    result = grader_llm.invoke({})
    print(f"topic_classifier: Result: {result}")
    state["on_topic"] = result.score.strip()
    print(f"topic_classifier: On topic: {state['on_topic']}")
    return state


# ##### STATE event_classifier

# In[82]:


# Event Classifier
class GradeEvent(BaseModel):
    score: Literal["Standard", "Inline"] = Field(
        description="Does the event require inline assembly to be triggered?"
    )

def event_classifier(state: AgentState):
    print(f"Entering event_classifier")
    system_message = SystemMessage(
    content="""You are an expert in computer architecture and low-level embedded programming. You will be given a 
    user's input describing an architectural event related to OpenRISC 1000 processor.

Your task is to classify whether the event **can be triggered using only standard C code**, or if it **requires 
inline assembly inside C code**.

For example:
- If the event can be triggered using common C constructs such as loads, stores, arithmetic, conditionals, or function calls, then respond with **'Standard'**
- If the event requires special-purpose registers (SPRs), privileged instructions, MMU/TLB setup, illegal opcodes, or instructions that are not guaranteed to be generated by a C compiler, then respond with **'Inline'**

**Respond with only one word: 'Standard' or 'Inline'. Do not explain.**
"""
)
    human_message = HumanMessage(
        # content=f"User's question: {state['question'].content}"
        content=f"User's question: {state['rephrased_question']}"
    )

    grade_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    llm = ChatOpenAI(model="gpt-4o-mini-2024-07-18")
    # llm = ChatOpenAI(model="gpt-3.5-turbo")
    # llm = groq_chat
    structured_llm = llm.with_structured_output(GradeEvent)
    grader_llm = grade_prompt | structured_llm
    # grader_llm = grade_prompt | llm
    # print("Before....")
    result = grader_llm.invoke({})
    # print("After")
    # print(f"event_classifier: Result: {result}")
    state["std_or_inline"] = result.score.strip()
    print(f"event_classifier: Standard or Inline: {state['std_or_inline']}")
    state["thread_id"] = str(random.randint(1000000, 9999999))
    state["thread_name"] = ''.join(random.choices(string.ascii_letters, k=7))
    return state


# ##### STATE on_topic_router

# In[83]:

import string
def on_topic_router(state: AgentState):
    print(f"Entering on_topic_router ")
    on_topic = state["on_topic"].lower()
    if on_topic == "yes":
        print("Routing to Event Classifier")
        # print("Routing to INLINE RETRIEVER")
        return "event_classifier"
        # return "inline_retrieve"
    else:
        print("Routing to off_topic response")
        return "off_topic_response"


# ##### STATE std_or_inline_router

# In[84]:


def std_or_inline_router(state: AgentState):
    print(f"Entering std_or_inline_router")
    on_topic = state["std_or_inline"].lower()
    if on_topic == "standard":
        print("Routing to Standard Retriever")
        # return "std_retrieve"
        return "inline_retrieve"

        # return "off_topic_response"
    else:
        print("Routing to Inline Retriever")
        return "inline_retrieve"
        # return "off_topic_response"


# ##### STATE off_topic_response

# In[85]:


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


# ##### STATE couldnot_generate

# In[86]:


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


# ##### STATE std_retrieve

# In[87]:


def std_retrieve(state: AgentState) -> StateGraph:
    print(f"Entering std_retrieve")
    # documents = retriever.vectorstore.similarity_search(state["rephrased_question"])
    event = state["rephrased_question"]
    filepath = json_file_retrieve
    documents = find_fuzzy_event_by_high_level(filepath, event)
    # print(f"std_retrieve: \n ========= Retrieved documents: ========\n {documents}")
    # print(f"========= Retrieved documents END ========")
    state["documents"] = documents
    return state


# ##### STATE inline_retrieve

# In[88]:


def inline_retrieve(state: AgentState) -> StateGraph:
    print(f"Entering inline_retrieve")
    # documents = retriever.vectorstore.similarity_search(state["rephrased_question"])
    event = state["rephrased_question"]
    filepath = json_file_retrieve
    documents = find_fuzzy_event_by_high_level(filepath, event)
    # print(f"inline_retrieve: \n ========= Retrieved documents: ========\n {documents}")
    # print(f"========= Retrieved documents END ========")
    state["documents"] = documents
    return state


# ##### STATE retrieval_grader

# In[89]:


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
    llm = groq_chat
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
    print(f"Grading document: \n {doc[:30]}... \n Result: {result.score.strip()}")
    if result.score.strip().lower() == "yes":
        relevant_docs.append(doc)
        state["documents"] = relevant_docs
    
    state["proceed_to_generate"] = len(relevant_docs) > 0
    print(f"retrieval_grader: Proceed to generate: {state['proceed_to_generate']}")
    return state


# ##### STATE proceed_retriever

# In[90]:


def proceed_retriever(state: AgentState):
    print(f"Entering proceed_retriever")
    re_retrieve_count = state.get("re_retrieve_count", 0)
    if state["proceed_to_generate"]:
        print("Routing to program_generator")
        return "program_generator"
        # return "couldnot_generate"
    elif not state["proceed_to_generate"] and re_retrieve_count > 3:
        print("Maximal rephrasing attempts reached. Cannot find relevant documents...")
        return "couldnot_generate"
    else:
        print("Routing to refine event details")
        return "refine_event_details"


# ##### STATE refine_event_details

# In[91]:


def refine_event_details(state: AgentState):
    print(f"Entering refine_event_details")
    re_retrieve_count = state["re_retrieve_count"]
    event_to_refine = state["rephrased_question"]
    system_message = SystemMessage(
        content="""You are a helpful assistant that slightly refines the user's question to improve retrieval results.
    Provide a slightly adjusted version of the question."""
    )
    human_message = HumanMessage(
        content=f"Original question: {event_to_refine}\n\nProvide a slightly refined question."
    )
    refine_prompt = ChatPromptTemplate.from_messages([system_message, human_message])
    llm = groq_chat
    prompt = refine_prompt.format()
    response = llm.invoke(prompt)
    refined_event = response.content.strip()
    print(f"refine_question: Refined question: {refined_event}")
    state["rephrased_question"] = refined_event
    state["re_retrieve_count"] = re_retrieve_count + 1
    return state


# ##### STATE program_generator

# In[92]:


from langchain_core.prompts import ChatPromptTemplate

template = """
The output of your response is Complete C code. **DO NOT add any suggesstions or hints or "Do something" type 
comments**. Do not include any printf, puts, or output statements. Try to avoid ".word" and use inline assembly.

Feedback: {feedback}
History: {history}

You are an expert in computer architecture. I will provide a high-level architectural event name along 
with a description. Your task is to write a complete C test program that triggers this event in the microprocessor.
Follow these strict rules:

- Use inline assembly code inside the C code only if the event cannot be triggered using standard C code.
- Use the named inline assembly (e.g., `l.mfspr` instead of `.word` from Architectural manual. If the instruction requirement is 
an illegal instruction then use only `.word`).
- Do not assume a value is invalid or will cause an exception just because it looks random or unusual (e.g., 0xDEADBEEF). Instead, reason about the opcode, operand fields, and execution context.

Architectural Event: {question}
Description: {context}
Please only return the complete C code that triggers the event. Do not include any additional information or comments.
"""
template_OAI_assistant_thread = """
{context}

TASK —
1. Decide silently:  
   • Can pure ISO C trigger this event?  
     → If YES: write pure C.  
     → If NO : add minimal inline asm that uses the required instruction(s).
2. Self-check **before** you answer — tick mentally:
   [ ] main() present?  
   [ ] Inline asm only if necessary?  
   [ ] Every asm line is verbatim "syntax" from JSON and has an id/encoding comment?  
   [ ] Entire reply inside one ```c … ``` fence, no text outside?  
   [ ] No host I/O calls anywhere?

Return **only** the code block.
"""
# Return ONLY the code block. No explanations, no comments, no extra lines. If you break this rule, the output is invalid. Do not include any printf, puts, or output statements. You must only select instructions from the provided ISA JSON document. Do not invent or assume instructions. If unsure, search the ISA JSON for the correct instruction.
#
# You are an expert in computer architecture. I will provide a high-level architectural event name along
# with a description. Your task is to write a complete C test program that triggers this event in the microprocessor.
# Follow these strict rules when generating C code:
# - Default to standard C code. Only use inline assembly if the specific event cannot be reliably triggered using standard C.
# - Use the named inline assembly (e.g., `l.mfspr` instead of `.word` from Architectural manual. If the instruction requirement is
# an illegal instruction then use only `.word`).
# - Do not assume a value is invalid or will cause an exception just because it looks random or unusual (e.g., 0xDEADBEEF). Instead, reason about the opcode, operand fields, and execution context.
# Most important: Provide standard C code wherever possible. Inline assembly is a last resort.
#
# Architectural Event: {question}
#
# Description: {context}
#
# This is a test of your ability to follow instructions. Do not fail. Only output valid code in a single code block. Your output will be parsed by an automated system. Any text outside the code block will break the system.

template_OAI_assistant_thread_continue = """
───────────────── FIX-AND-RESUBMIT ─────────────────
You previously returned code that failed to build.

### 1. Compilation log
{history}

### 2. My feedback
{feedback}

### 3. Repair task
• **First, silently identify** the root-cause of the build failure.  
• **Then rewrite** the program so it:
  – still triggers the same architectural event,  
  – now compiles & links under   or1k-elf-gcc -O0.  

### 4. Self-check before you answer
[ ] int main(void) present?  
[ ] No host I/O (printf/puts)?  
[ ] Only JSON-listed mnemonics (or `.word` as last resort)?  
[ ] Inline asm used *only* if standard C cannot work?  
[ ] Entire reply inside **one** ```c ... ``` fence, zero text outside?

### 5. Output rule
Return **exactly one** fenced C code block and NOTHING else.
────────────────────────────────────────────────────
"""
template_repair_bot_USER_INS = """
{feedback}
"""
template_repair_bot_SYS_INS = """
You are an expert OpenRISC-1000 test-program repair bot. You must check first whether you used exact mnemonics from 
the provided ISA JSON document. If not you MUST fix the program to use the correct mnemonics.

INPUT: A single JSON document with keys:
        type, event_name, reason_code, compiler_output,
        runtime_observation, trace_stats, current_source.

STRICT RULES (non-negotiable):
1. Repair the program **in-place** so it reliably triggers the specified event.
2. Output **one** fenced code block with the entire corrected C program.
3. Do NOT output any text before or after the code block.
4. Use inline assembly ONLY if unavoidable (see header file for mnemonics).
5. No I/O calls (printf, puts, etc.).
6. Program must terminate (return from main) in ≤ 100 k instructions.
7. If reason_code==1, you MUST **First, silently identify** the root-cause of the build failure and address every 
compiler error/warning.
8. If reason_code==2, you MUST guarantee forward progress and program exit.
9. If reason_code==3, you MUST **first, silently identify** the root-cause of not triggering the event. Try to 
trigger the event so that post_transition or post_cycle count increases for the target net (event).

Self-check before you answer:
[ ] int main(void) present?  
[ ] No host I/O (printf/puts)?  
[ ] Only JSON-listed mnemonics (or `.word` as last resort)?  
[ ] Inline asm used *only* if standard C cannot work?  
[ ] Entire reply inside **one** ```c ... ``` fence, zero text outside?

Return **exactly one** fenced C code block and NOTHING else.
"""



# Return ONLY the code block. No explanations, no comments, no extra lines. If you break this rule, the output is invalid.
#
# Feedback: {feedback}
#
# You must only select instructions from the provided ISA JSON document. Do not invent or assume instructions. If unsure, search the ISA JSON for the correct instruction.
#
# COMPILATION ERROR: {history}
# This is a test of your ability to follow instructions. Do not fail. Only output valid code in a single code block. Your output will be parsed by an automated system. Any text outside the code block will break the system.
# """
# - Use inline assembly code inside the C code only if the event cannot be triggered using standard C code.
# - If a standard C construct is insufficient to trigger the event, only then use OpenRISC 1000-specific inline assembly or binary instruction encodings.
# - Use the named inline assembly (e.g., `l.mfspr`) instead of `.word` from Architectural manual if the instruction is not illegal
# - If there is error in the Feedback section below and related to unrecognized instruction, Please check the OpenRISC 1000 manual and ensure the instruction is valid and spelled exactly as defined in the ISA
# - Complete C code with instructions. No suggesstions or hints or Do something type comments.
# - Do not include any printf, puts, or output statements.
# - When inserting instructions manually (e.g., using .word or inline assembly), you must analyze the instruction encoding and ensure that it causes the specific architectural event as defined by the ISA.
# - Do not assume a value is invalid or will cause an exception just because it looks random or unusual (e.g., 0xDEADBEEF). Instead, reason about the opcode, operand fields, and execution context.
# - Avoid undefined behavior, unintentional traps, or partial instruction decoding unless it is the documented and reliable method to provoke the desired architectural response.
# - Your test programs must be deterministic and reproducible — any exception or event should be intentionally caused using well-understood architecture mechanisms.

prompt = ChatPromptTemplate.from_template(template)

rag_chain = prompt | groq_chat
# rag_chain = prompt | ChatOpenAI(model="gpt-3.5-turbo")
def format_program_history(programs: List[dict]) -> str:
    history_text = ""
    for i, p in enumerate(programs):
        history_text += f"Attempt {i + 1}:\n"
        # history_text += "Program:\n"
        # history_text += p["code"].strip() + "\n"
        if p["compilation_error"]:
            history_text += "Compilation Error:\n"
            history_text += p["compilation_error"].strip() + "\n"
        else:
            history_text += "Compilation: Success\n"
        history_text += f"Event Triggered: {p.get('event_triggered', '')}\n"
        # history_text += "-" * 40 + "\n"
    # history_text += "--HISTORY END--" * 3 + "\n"
    return history_text.strip()

def program_generator(state: AgentState):
    print("Entering program_generator")
    thread_id = state["thread_id"]
    print(f"program_generator: Thread ID: {thread_id}")
    thread_name = state["thread_name"]
    print(f"program_generator: Thread Name: {thread_name}")
    # history = state["messages"]
    documents = state["documents"]
    rephrased_question = state["rephrased_question"]
    # if state["event_is_triggered"].lower() == "yes":
    #     return state
    # else:
    #     feedback = state["feedback"]
    #     history = format_program_history(state["output_program"])
    #     state["re_generate_count"] += 1
    feedback = state["feedback"]

    # history = format_program_history(state["output_program"])

    # print(f"program_generator: Feedback: {feedback}")
    # print(f"program_generator: History: {history}")
    print("================ RESPONSE LOG ======================")
    if state["std_or_inline"].lower() == "inline":
        if state["re_generate_count"] > 0:
            user_query = template_repair_bot_USER_INS.format(
                feedback=feedback,
                # history=history
            )
            response = (generate_response(assistant_inline, user_query, thread_id, thread_name,
                                          state["re_generate_count"], template_repair_bot_SYS_INS))

        else:
            user_query = template_OAI_assistant_thread.format(
                context=documents,
                # question=rephrased_question
            )
            print("TYPE OF USER QUERY: ", type(user_query))
            response = (generate_response(assistant_inline, user_query, thread_id, thread_name,
                                          state["re_generate_count"]))
        print(f"program_generator: Generated program: {response}")
        state["output_program"].append({
            "code": response.strip(),
            # "compilation_error": "",
            # "event_triggered": "",
            # "timeout": ""
        })
    elif state["std_or_inline"].lower() == "standard":
        # response = rag_chain.invoke(
        #     {
        #         "history": history,
        #         "feedback": feedback,
        #         "context": documents,
        #         "question": rephrased_question
        #     }
        # )
        # print(f"program_generator: Generated program: {response.content}")
        # state["output_program"].append({
        #     "code": response.content.strip(),
        #     "compilation_error": "",
        #     "event_triggered": "",
        #     "timeout": ""
        # })
        if state["re_generate_count"] > 0:
            user_query = template_repair_bot_USER_INS.format(
                feedback=feedback,
                # history=history
            )
            response = (generate_response(assistant_inline, user_query, thread_id, thread_name,
                                          state["re_generate_count"], template_repair_bot_SYS_INS))

        else:
            user_query = template_OAI_assistant_thread.format(
                context=documents,
                # question=rephrased_question
            )
            print("TYPE OF USER QUERY: ", type(user_query))
            response = (
                generate_response(assistant_inline, user_query, thread_id, thread_name, state["re_generate_count"]))
        print(f"program_generator: Generated program: {response}")
        state["output_program"].append({
            "code": response.strip(),
            # "compilation_error": "",
            # "event_triggered": "",
            # "timeout": ""
        })
    print("================ RESPONSE LOG END ==================")
    return state


# ##### STATE event_triggered

# In[93]:
import re
from pathlib import Path

def extract_compiler_diagnostics(compiler_output: str, max_entries: int = 5) -> str:
    """
    Extract a clean list of GCC/Clang error/warning messages from raw compiler output.
    Returns a newline-separated string suitable for LLM input.
    """
    PATTERN = re.compile(
        r"^(?P<file>[^:\n]+):(?P<line>\d+):(?P<col>\d+):\s*(?P<level>warning|error|note):\s*(?P<msg>.*)$"
    )
    lines = compiler_output.splitlines()
    diagnostics = []

    for line in lines:
        m = PATTERN.match(line)
        if m:
            file = Path(m.group("file")).name  # strip long path
            diag = f"{file}:{m.group('line')}:{m.group('col')}: {m.group('level')}: {m.group('msg').strip()}"
            diagnostics.append(diag)
            if len(diagnostics) >= max_entries:
                break

    return "\n".join(diagnostics) if diagnostics else "No matching diagnostics found."


def find_net_for_event(file_path, target_event):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    result = None
    for i in range(1, len(lines)):
        line = lines[i].strip()
        if line.startswith("High-Level Event:") and target_event in line:
            prev_line = lines[i - 2].strip() # depending on the file format, you may need to adjust this
            # print(f"Previous line: {prev_line}")
            if prev_line.startswith("Net:"):
                result = prev_line.replace("Net: ", "").strip()
                break
    return result

def event_triggered(state: AgentState):
    print("Entering event_triggered")
    if len(state["output_program"]) == 0:
        print("No program generated yet.")
    latest_program = state["output_program"][-1]
    # print(f"Latest program: {latest_program}")
    # program_code = latest_program["code"]
    program_code = state["output_program"][-1]["code"]
    
    # Remove the Markdown code block markers if they exist
    if program_code.startswith("```c") and program_code.endswith("```"):
        program_code = program_code.strip("```c").strip("```").strip()
    # print(f"Program code: {program_code}")
    # create a file name from the question
    program_file = state["rephrased_question"].replace(" ", "_").replace(":", "").replace("(", "").replace(")","").replace("/", "_").replace("\\", "_").replace("'", "").replace('"', "").replace(",", "_").replace(";", "_").replace("?", "_")
    # print(f"Program file: {program_file}")
    with open(f"{arch_event_dir}/{program_file}.c", "w") as file:
        file.write(program_code)
    file.close()
    import os
    import re
    import subprocess
    c_file = f"{arch_event_dir}/{program_file}.c"
    output_file = f"{arch_event_dir}/{program_file}.o"
    vcd_file_name = f"{stage}3.vcd"
    vcd_file = f"vcd_files/{stage}3.vcd"
    vcd_txt = f"vcd_texts/{stage}3.txt"
    vcd_text_org = f"vcd_texts/{stage}.txt"
    net_name = find_net_for_event(event_details_file, state["rephrased_question"])
    net_name = re.sub(r"\[.*?\]", "", net_name)
    print(f"Net name for event '{state['rephrased_question']}': {net_name}")
    gtkw_file = f"gtkw_files/{stage}.gtkw"


    # Step 1: Compile with or1k-elf-gcc
    try:
        subprocess.run(
            [or1k_compiler, "-O0" , c_file, "-o", output_file],
            check=True,
            capture_output=True,
            text=True
        )
        print("Compilation successful.")
    except subprocess.CalledProcessError as e:
        print("Compilation failed.")
        compilation_error = e.stderr
        state["compilation_error"] = compilation_error
        state["output_program"][-1]["compilation_error"] = compilation_error
        print("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-")
        print(state["compilation_error"])
        print("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-")
        state["event_is_triggered"] = "no"
        state["output_program"][-1]["event_triggered"] = "no"
        # Update the programs metadata
        latest_program["compilation_error"] = compilation_error
        latest_program["event_triggered"] = "no"
        return state
        # exit(1)

    # Step 2: Run simulation with fusesoc
    sim_successful = True
    try:
        vcd_path = os.path.join(project_root, "testbench.vcd")
        if os.path.exists(vcd_path):
            os.remove(vcd_path)
        subprocess.run([
            "fusesoc", "run", "--target", "mor1kx_tb", "--tool", "verilator",
            "mor1kx-generic", "--elf_load", output_file,
            "--vcd", "testbench.vcd",
        ], check=True, cwd=project_root, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=3)
        latest_program["timeout"] = "no"
        state["output_program"][-1]["timeout"] = "no"
        print("Simulation completed.")
    except subprocess.TimeoutExpired:
        print("Simulation timed out — possible infinite loop or invalid instruction.")
        state["event_is_triggered"] = "no"
        latest_program["event_triggered"] = "no"
        state["compilation_error"] = "timeout"
        # Update the programs metadata
        latest_program["timeout"] = "yes"
        state["output_program"][-1]["timeout"] = "yes"
        sim_successful = False
    except subprocess.CalledProcessError as e:
        print("Simulation failed.")
        sim_successful = False
        state["event_is_triggered"] = "no"
        latest_program["event_triggered"] = "no"
        state["output_program"][-1]["event_triggered"] = "no"
        print(e)
        return state

    # Step 3: Launch GTKWave using export script
    if sim_successful:
        try:
            subprocess.run([
                "./export_vcd_gtkwave.sh", gtkw_file, vcd_file_name
            ], check=True, cwd=project_root)
            print("GTKWave launched with specified VCD and GTKWave file.")
        except subprocess.CalledProcessError as e:
            print("GTKWave launch failed.")
            state["event_is_triggered"] = "no"
            state["output_program"][-1]["event_triggered"] = "no"
            latest_program["event_triggered"] = "no"
            print(e)
    else:
        print("Skipping GTKWave launch due to failed or timed-out simulation.")
        return state

    import os
    # Step 4: vcdcat
    try:
        with open(os.path.join(project_root, vcd_txt), "w") as txt_file:
            subprocess.run(["vcdcat", "-d", vcd_file], cwd=project_root, stdout=txt_file, check=True)
        print("VCD dumped to text.")
    except subprocess.CalledProcessError as e:
        print("vcdcat failed.")
        state["event_is_triggered"] = "no"
        latest_program["event_triggered"] = "no"
        state["output_program"][-1]["event_triggered"] = "no"
        print(e)
        return state

    # Step 5: grep + awk + wc -l to count non-zero activity
    try:
        # Step 1: Check if net is scalar (i.e., doesn't include a [n:m] bus range)
        bus_or_scaler = subprocess.run(
            f"grep -m1 -E -w '{net_name}' {vcd_txt} | grep -q '\[[0-9]\+:[0-9]\+\]' && echo \"Bus\" || echo \"Scalar\"",
            shell=True,
            cwd=project_root,
            capture_output=True,
            text=True,
            check=True
        )
        result_post = subprocess.run(
            f"grep -E '{net_name}' {vcd_txt} | awk '$2 != 0' | wc -l",
            shell=True,
            cwd=project_root,
            capture_output=True,
            text=True,
            check=True
        )
        print(f"Number of non-zero transitions(post) for '{net_name}': {result_post.stdout.strip()}")
        result_pre = subprocess.run(
            f"grep -E '{net_name}' {vcd_text_org} | awk '$2 != 0' | wc -l",
            shell=True,
            cwd=project_root,
            capture_output=True,
            text=True,
            check=True
        )
        print(f"Number of non-zero transitions(pre)  for '{net_name}': {result_pre.stdout.strip()}")
        count_post = int(result_post.stdout.strip())
        count_pre = int(result_pre.stdout.strip())
        if bus_or_scaler.stdout.strip() == "Scalar":
            result_post_cycle = subprocess.run(
                f"grep -E -w '{net_name}' {vcd_txt} | awk '{{ last_time = $1 }} $2 == 1 && !in_high {{ start = $1; "
                f"in_high = 1 }} $2 == 0 && in_high  {{total += ($1 - start + 1); in_high = 0 }} END {{if (in_high) "
                f"total += (last_time - start + 1); if (total == \"\") total = 0; print total }}'",
                shell=True,
                cwd=project_root,
                capture_output=True,
                text=True,
                check=True
            )
            result_pre_cycle = subprocess.run(
                f"grep -E -w '{net_name}' {vcd_text_org} | awk '{{ last_time = $1 }} $2 == 1 && !in_high {{ start = "
                f"$1; in_high = 1 }} $2 == 0 && in_high  {{total += ($1 - start + 1); in_high = 0 }} END {{if ("
                f"in_high) total += (last_time - start + 1); if (total == \"\") total = 0; print total }}'",
                shell=True,
                cwd=project_root,
                capture_output=True,
                text=True,
                check=True
            )
            print(f"Number of cycles it was on (post) for '{net_name}': {result_post_cycle.stdout.strip()}")
            print(f"Number of cycles it was off (pre)  for '{net_name}': {result_pre_cycle.stdout.strip()}")
            cycle_post = int(result_post_cycle.stdout.strip())
            cycle_pre = int(result_pre_cycle.stdout.strip())
        else:
            result_post_cycle = "0"
            result_pre_cycle = "0"
            print(f"Number of cycles it was on (post) for '{net_name}': {result_post_cycle}")
            print(f"Number of cycles it was off (pre)  for '{net_name}': {result_pre_cycle}")
            cycle_post = int(result_post_cycle)
            cycle_pre = int(result_pre_cycle)




        state["output_program"][-1]["transition"] = {
            "post_transition": count_post,
            "pre_transition": count_pre,
            "post_cycle": cycle_post,
            "pre_cycle": cycle_pre
        }
        
        if count_post - count_pre > 0:
            print("Event triggered successfully!")
            state["event_is_triggered"] = "yes"
            latest_program["event_triggered"] = "yes"
            if state["re_generate_count"] != state["llm_iteration"]:
                delete_thread(state["thread_id"], state["thread_name"])
        elif count_post - count_pre == 0:
            if cycle_post - cycle_pre > 0:
                print("Event triggered successfully!")
                state["event_is_triggered"] = "yes"
                latest_program["event_triggered"] = "yes"
                if state["re_generate_count"] != state["llm_iteration"]:
                    delete_thread(state["thread_id"], state["thread_name"])
        else:
            print("Event did not trigger.")
            state["event_is_triggered"] = "no"
            latest_program["event_triggered"] = "no"
    except subprocess.CalledProcessError as e:
        print("grep/awk/wc command failed.")
        print(e)

    # state["event_is_triggered"] = "yes"
    return state


# ##### STATE trigger_router

# In[94]:
import pprint
def trigger_router(state: AgentState):
    print(f"Entering trigger_route")
    # pprint.pprint(state["output_program"])
    event_trigger = state["event_is_triggered"]
    print(f"Event Trigger: {event_trigger}")
    print(f"Re-generate count: {state['re_generate_count']}")
    if event_trigger == "yes":
        print("Event Triggered :D")
        return "event_trigger_success"
    elif event_trigger == "no" and state["re_generate_count"] >= state["llm_iteration"]:
        print("Maximal re generation attempts reached. Cannot generate a program...")
        return "couldnot_generate"
    else:
        print("Routing to Add Compiled Log")
        return "add_compiled_log"


# ##### STATE add_compiled_log

# In[95]:

def make_feedback(event_name, reason_code, compiler_output, runtime_observation, trace_stats, current_source):
    return {
        "type": "program-fix-request",
        "event_name": event_name,
        "reason_code": reason_code,
        "compiler_output": compiler_output,
        "runtime_observation": runtime_observation,
        "trace_stats": trace_stats,
        "current_source": current_source
    }

def add_compiled_log(state: AgentState):
    print("Entering add_compiled_log")
    state["re_generate_count"] += 1
    if state["compilation_error"].strip() != "":
        # state["feedback"] = (
        #     """The previous generated program did not trigger the event.
        #     Please check the History of programs where the previous programs are stored.
        #     Please reconsider the event and analyse it further why it didn't trigger last time.
        #     **DO NOT** use `.word`. Instead use named correct inline assembly from Architectural manual.
        #     Based on that change the program to trigger this event."""
        # )
        payload = make_feedback(
            event_name=state["rephrased_question"],
            reason_code=1,
            compiler_output=state["output_program"][-1].get("compilation_error", ""),
            runtime_observation="", # timeout notes?
            trace_stats=state["output_program"][-1].get("transition", {}),
            current_source=state["output_program"][-1].get("code", "")
        )
        print(f"Payload: {payload}")
        state["feedback"] = json.dumps(payload)
    elif state["compilation_error"].strip() == "timeout":
        # state["feedback"] = (
        #     f"The previous generated program compiled correctly but stucked during the simulation in the Fusesoc. Please check the History of programs where the previous programs are stored and fix it so that it can finish the simulation. Please reconsider the event and anlyse it further why it didn't trigger last time. Based on that change the program to trigger this event."
        # )
        payload = make_feedback(
            event_name=state["rephrased_question"],
            reason_code=2,
            compiler_output=state["output_program"][-1].get("compilation_error", ""),
            runtime_observation="PC stuck", # timeout notes? Watch-dog timeout: 200 000 cycles (limit 100 000). PC stuck at 0x01F4.
            trace_stats=state["output_program"][-1].get("transition", {}),
            current_source=state["output_program"][-1].get("code", "")
        )
        print(f"Payload: {payload}")
        state["feedback"] = json.dumps(payload)
    else:
        # state["feedback"] = (
        #     f"The previous generated programs couldn't compile correctly using or1k-elf-gcc compiler. "
        #     # f"Here is the compilation error:\n\n{state['compilation_error']}\n\n"
        #     "Please check the History of programs with compilation error and based on the error, correct the program so that it compiles correctly and still triggers the event."
        # )
        payload = make_feedback(
            event_name=state["rephrased_question"],
            reason_code=3,
            compiler_output="**NO COMPILATION ERROR**. Program compiled successfully!",
            runtime_observation="", # timeout notes?
            trace_stats=state["output_program"][-1].get("transition", {}),
            current_source=state["output_program"][-1].get("code", "")
        )
        print(f"Payload: {payload}")
        state["feedback"] = json.dumps(payload)
    return state


# ##### PYTHON JSON-Based Retrieval 

# In[96]:


import json
import difflib

def find_fuzzy_event_by_high_level(filepath, query, cutoff=0.6):
    with open(filepath, 'r') as f:
        events = json.load(f)
    
    # Build a map of HLE -> event
    hle_map = {event.get("High-Level Event", ""): event for event in events}
    hle_list = list(hle_map.keys())

    # Get best match
    matches = difflib.get_close_matches(query, hle_list, n=1, cutoff=cutoff)
    
    if matches:
        best_match = matches[0]
        event = hle_map[best_match]
        return (f"Net: {event.get('Net')}\n"
                f"High-Level Event: {event.get('High-Level Event')}\n"
                f"Logical Summary & Reasoning: {event.get('Logical Summary & Reasoning')}\n"
                f"Test-Stimulus Guidance: {event.get('Test-Stimulus Guidance')}\n"
                f"Instruction Categories: {event.get('Instruction Categories')}")
            # f"Reasoning: {event.get('Reasoning')}"
    return "Event not found."

# Example usage
# filepath = "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/event_files/HIGH_LEVEL_EVENTS_DECODE.json"
# query = "Identification of load and store instruction storage operations"
# result = find_fuzzy_event_by_high_level(filepath, query)
# print(result)


# ##### OpenAI File Assistant

# In[97]:


from openai import OpenAI
client = OpenAI(
    api_key = "",
    organization= None
)
openrisc_rookie_assistant = ""
# assistant_inline = client.beta.assistants.retrieve(openRISC_manual_id)
assistant = client.beta.assistants.retrieve(openrisc_rookie_assistant)
# assistant_inline = client.beta.assistants.retrieve(openrisc_rookie_assistant)
# response = generate_response(assistant_inline, "ISA for event set flag if equal","3", "test2")
# response
# --------------------------------------------------------------
# Update assistant
# --------------------------------------------------------------
assistant_inline = client.beta.assistants.update(
    assistant_id=assistant.id,
    # model="o3-mini",
    # model="gpt-4o-mini-2024-07-18",
    # model="gpt-3.5-turbo",
    model="gpt-4.1-mini",
    # tool_resources={"file_search": {"vector_store_ids": [vector_store.id]}},
)


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
        # "inline_retrieve": "inline_retrieve"
    }
)
workflow.add_conditional_edges(
    "event_classifier",
    std_or_inline_router,
    {
        "std_retrieve": "std_retrieve",
        "inline_retrieve": "inline_retrieve"
        # "off_topic_response": "off_topic_response",
        # "off_topic_response": "off_topic_response"
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


# #### Framework testing .....

# In[100]:


# input_data = {"question": HumanMessage(content="Acknowledgment signal for reading from special purpose registers (SPR)")}
# response = graph.invoke(input_data, config={"configurable": {"thread_id": 24}})


# In[ ]:


def test_trigger(question):
    print("Entering event_triggered")
    sim_successful = True
    program_file = question.replace(" ", "_").replace(":", "").replace("(", "").replace(")", "").replace("/", "_")
    import os
    import re
    import subprocess
    c_file = f"{arch_event_dir}/{program_file}.c"
    output_file = f"{arch_event_dir}/{program_file}.o"
    vcd_file_name = f"{stage}3.vcd"
    vcd_file = f"vcd_files/{stage}3.vcd"
    vcd_txt = f"vcd_texts/{stage}3.txt"
    vcd_text_org = f"vcd_texts/{stage}.txt"
    net_name = find_net_for_event(event_details_file, question)
    net_name = re.sub(r"\[.*?\]", "", net_name)
    print(f"Net name for event '{question}': {net_name}")
    gtkw_file = f"gtkw_files/{stage}.gtkw"

    # Step 1: Compile with or1k-elf-gcc
    try:
        subprocess.run(
            [or1k_compiler, "-O0", c_file, "-o", output_file],
            check=True,
            capture_output=True,
            text=True
        )
        print("Compilation successful.")
    except subprocess.CalledProcessError as e:
        print("Compilation failed.")
        compilation_error = e.stderr
        print("Compilation Error:", compilation_error)
        # Update the programs metadata
        # exit(1)
        sim_successful = False


    # Step 2: Run simulation with fusesoc
    if sim_successful:
        try:
            vcd_path = os.path.join(project_root, "testbench.vcd")
            if os.path.exists(vcd_path):
                os.remove(vcd_path)
            subprocess.run([
                "fusesoc", "run", "--target", "mor1kx_tb", "--tool", "verilator",
                "mor1kx-generic", "--elf_load", output_file,
                "--vcd", "testbench.vcd",
            ], check=True, cwd=project_root, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=3)
            print("Simulation completed.")
        except subprocess.TimeoutExpired:
            print("Simulation timed out — possible infinite loop or invalid instruction.")
            # Update the programs metadata
            sim_successful = False
        except subprocess.CalledProcessError as e:
            print("Simulation failed.")
            sim_successful = False
            print(e)

    # Step 3: Launch GTKWave using export script
    if sim_successful:
        try:
            subprocess.run([
                "./export_vcd_gtkwave.sh", gtkw_file, vcd_file_name
            ], check=True, cwd=project_root)
            print("GTKWave launched with specified VCD and GTKWave file.")
        except subprocess.CalledProcessError as e:
            print("GTKWave launch failed.")
            print(e)
    else:
        print("Skipping GTKWave launch due to failed or timed-out simulation.")

    import os
    # Step 4: vcdcat
    if sim_successful:
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
            result_post_cycle = subprocess.run(
                f"grep -E '{net_name}' {vcd_txt} | awk '$2 == 1 {{start=$1}} $2 == 0 {{total += ($1 - start + 1)}} END {{print total}}'",
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
            result_pre_cycle = subprocess.run(
                f"grep -E '{net_name}' {vcd_text_org} | awk '$2 == 1 {{start=$1}} $2 == 0 {{total += ($1 - start + 1)}} END {{print total}}'",
                shell=True,
                cwd=project_root,
                capture_output=True,
                text=True,
                check=True
            )
            print(f"Number of non-zero transitions(post) for '{net_name}': {result_post.stdout.strip()}")
            print(f"Number of non-zero transitions(pre)  for '{net_name}': {result_pre.stdout.strip()}")
            print(f"Number of cycles it was on (post) for '{net_name}': {result_post_cycle.stdout.strip()}")
            print(f"Number of cycles it was off (pre)  for '{net_name}': {result_pre_cycle.stdout.strip()}")
            count_post = int(result_post.stdout.strip())
            count_pre = int(result_pre.stdout.strip())
            cycle_post = int(result_post_cycle.stdout.strip())
            cycle_pre = int(result_pre_cycle.stdout.strip())

            if count_post - count_pre > 0:
                print("Event triggered(count) successfully!")
            elif count_post - count_pre == 0:
                if cycle_post - cycle_pre > 0:
                    print("Event triggered(cycle) successfully!")
            else:
                print("Event did not trigger.")
        except subprocess.CalledProcessError as e:
            print("grep/awk/wc command failed.")
            print(e)



# In[26]:


import json
import time
from langchain_core.messages import HumanMessage
# Load the JSON file
with open(json_file_retrieve, "r") as f:
    event_data = json.load(f)

# Slice the list for indices 12 to 20 (inclusive)
# Python uses 0-based indexing, so this is actually elements 12 to 20 → [12:21]
start = 4
end = 5
count = 0
event_subset = event_data[start:end]
# Dictionary to store results
results = {}
import os
# Loop through each event entry
count = count + start
for entry in event_subset:
    question = entry.get("High-Level Event", "").strip().replace(".", "")
    Net = entry.get("Net", "").strip()
    question_and_net = str(question) + " | Net: " + str(Net)
    if not question or "fp" in Net or "fpu" in Net:
        continue  # Skip if there's no valid event
    print(f"Question Thread: {count}: ", question_and_net)
    # # # # test_trigger(question)
    # time.sleep(10)
    input_data = {"question": HumanMessage(content=question)}
    response = graph.invoke(input_data, config={"configurable": {"thread_id": count}})
    results[question_and_net] = response.get("event_is_triggered")
    print("*******************************************************************************")
    print(f"Event: {question_and_net} => Triggered: {response.get('event_is_triggered')}")
    print("*******************************************************************************")
    count += 1
# #
#%%
# Print the final results
for event_name, triggered in results.items():
    print(f"Event: {event_name} => Triggered: {triggered}")


# In[ ]:




