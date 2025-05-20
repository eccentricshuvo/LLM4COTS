int main() {
    asm volatile (
        "l.j 0x00000004"   // Jump instruction to trigger Jump Branch Instruction Detection
    );
    return 0;
}