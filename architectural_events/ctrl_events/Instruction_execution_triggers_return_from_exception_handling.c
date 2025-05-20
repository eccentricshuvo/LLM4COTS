int main() {
    // Trigger return from exception handling
    asm volatile(".word 0xDEADBEEF"); // Invalid instruction to trigger exception handling
    return 0;
}