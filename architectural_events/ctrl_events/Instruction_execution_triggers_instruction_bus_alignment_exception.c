int main(void) {
    // Triggering an instruction bus alignment exception using inline assembly
    asm(".word 0x60000000"); // Instruction to trigger alignment exception

    return 0;
}