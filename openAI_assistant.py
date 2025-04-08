##################################################################
from langchain_openai import OpenAIEmbeddings, ChatOpenAI
from langchain_groq import ChatGroq
from groq import Groq
from langchain.llms.base import LLM

# Step 1: Set up the OpenAI LLM
llm_OAI = ChatOpenAI(
    #model="gpt-4-turbo-preview", # "gpt-4" or "gpt-3.5-turbo"
    model="gpt-3.5-turbo",
    openai_api_key="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",  # Replace with your OpenAI API key
    temperature=0.7,  # Adjust the creativity level
    max_tokens=500,   # Set the maximum output token limit
    verbose=True
)
from openai import OpenAI
client = OpenAI(
    api_key = "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",
    organization= None
)

####################################################################
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
# ######################################################################
# #%%
# # --------------------------------------------------------------
# # Upload file
# # --------------------------------------------------------------
# def upload_file(path):
#     # Upload a file with an "assistants" purpose
#     file = client.files.create(file=open(path, "rb"), purpose="assistants")
#     return file
#
#
# #file = upload_file("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/openrisc-arch-1.4-rev0.pdf")
# file = upload_file("/home/m588h354/projects/Rare_net_analysis-repo/event_identification/decode_events.txt")
# file_id = file.id
# print(file_id)
# #%%
# # --------------------------------------------------------------
# # Create assistant
# # --------------------------------------------------------------
# assistant = client.beta.assistants.create(
#     name="OpenRISC 1000 Architecture Manual Assistant",
#     instructions="""
#     You are an expert on the OpenRISC 1000 computer architecture and know details about all the architectural
#     events in a microprocessor. You deeply understand RISC design principles, including the register file,
#     the instruction set structure, memory models, privilege levels, and exception
#     handling mechanisms. Base on the architectural events you also know how to generate test programs
#     that can intentionally trigger those architectural events. I have been developing the pdf over the years as a contributor
#     and giving you it to further understand.
#     """,
#     model="gpt-3.5-turbo",
#     tools=[{"type": "file_search"}]
# )
# #%%
# # --------------------------------------------------------------
# # Upload files in batch
# # --------------------------------------------------------------
# # Create a vector store caled "OpenRISC 1000 Architectural Manual"
# vector_store = client.beta.vector_stores.create(name="OpenRISC 1000 Architectural Manual")
#
# # Ready the files for upload to OpenAI
# file_paths = ["/home/m588h354/projects/Rare_net_analysis-repo/event_identification/openrisc-arch-1.4-rev0.pdf",
# "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_decode.txt",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx-defines.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx-sprs.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_fetch_cappuccino.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_icache.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_immu.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_simple_dpram_sclk.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/mor1kx_true_dpram_sclk.v",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/or1k-sprs.h",
# # "/home/m588h354/projects/Rare_net_analysis-repo/event_identification/or1k-sprs.h"
#               ]
# file_streams = [open(path, "rb") for path in file_paths]
#
# # Use the upload and poll SDK helper to upload the files, add them to the vector store,
# # and poll the status of the file batch for completion.
# file_batch = client.beta.vector_stores.file_batches.upload_and_poll(
#   vector_store_id=vector_store.id, files=file_streams
# )
#
# # You can print the status and the file counts of the batch to see the result of this operation.
# print(file_batch.status)
# print(file_batch.file_counts)
# #%%
# # --------------------------------------------------------------
# # Update assistant
# # --------------------------------------------------------------
# assistant = client.beta.assistants.update(
#     assistant_id=assistant.id,
#     model="gpt-3.5-turbo",
#     tool_resources={"file_search": {"vector_store_ids": [vector_store.id]}},
# )
# assistant.id
#%%
import shelve
# --------------------------------------------------------------
# Thread management
# --------------------------------------------------------------
def check_if_thread_exists(wa_id):
    with shelve.open("threads_db") as threads_shelf:
        return threads_shelf.get(wa_id, None)


def store_thread(wa_id, thread_id):
    with shelve.open("threads_db", writeback=True) as threads_shelf:
        threads_shelf[wa_id] = thread_id


#%%
import time
# --------------------------------------------------------------
# Run assistant
# --------------------------------------------------------------
def run_assistant(thread,assistant):
    # Retrieve the Assistant
    assistant = client.beta.assistants.retrieve(assistant.id)

    # Run the assistant
    run = client.beta.threads.runs.create(
        thread_id=thread.id,
        assistant_id=assistant.id,
    )

    # Wait for completion
    while run.status != "completed":
        # Be nice to the API
        time.sleep(0.5)
        run = client.beta.threads.runs.retrieve(thread_id=thread.id, run_id=run.id)

    # Retrieve the Messages
    messages = client.beta.threads.messages.list(thread_id=thread.id)
    new_message = messages.data[0].content[0].text.value
    # print(f"Generated message: {new_message}")
    return new_message
#%%
# --------------------------------------------------------------
# Generate response
# --------------------------------------------------------------
def generate_response(assistant, message_body, wa_id, name):
    # Check if there is already a thread_id for the wa_id
    thread_id = check_if_thread_exists(wa_id)

    # If a thread doesn't exist, create one and store it
    if thread_id is None:
        print(f"Creating new thread for {name} with wa_id {wa_id}")
        thread = client.beta.threads.create()
        store_thread(wa_id, thread.id)
        thread_id = thread.id

    # Otherwise, retrieve the existing thread
    else:
        print(f"Retrieving existing thread for {name} with wa_id {wa_id}")
        thread = client.beta.threads.retrieve(thread_id)

    # Add message to thread
    message = client.beta.threads.messages.create(
        thread_id=thread_id,
        role="user",
        content=message_body,
    )

    # Run the assistant and get the new message
    new_message = run_assistant(thread, assistant)
    # print(f"To {name}:", new_message)
    return new_message

# #%%
# response = generate_response("ISA for event set flag if equal", "1", "test1")
# #%%
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
# query = get_retriever_prompt("pipeline_flush_i")
#
# #%%
# response = generate_response(query, "2", "test1")
# response