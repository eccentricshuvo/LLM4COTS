void illegal_instr_exception_test() {
    // Emit illegal instruction: will trigger exception → flush
    asm volatile (".word 0xFFFFFFFF");  // Invalid opcode, triggers exception
}

int main() {
    illegal_instr_exception_test();
    return 0;
}

