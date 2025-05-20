int main() {
    // Triggering operation control for RFE (Return from Exception) using inline assembly
    asm volatile(
        ".long 0xE000000D  /* l.trap 0, 13 */" // Causing an exception with l.trap instruction
    );

    return 0;
}