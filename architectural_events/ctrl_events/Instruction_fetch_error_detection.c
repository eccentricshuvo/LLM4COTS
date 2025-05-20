int main() {
    // Inline assembly to cause an instruction fetch error
    asm volatile(".word 0xFFFF0000"); // Invalid memory location access

    return 0;
}