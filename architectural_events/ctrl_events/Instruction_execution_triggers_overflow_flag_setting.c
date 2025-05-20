int main() {
    int a = 2147483647; // Maximum value for a signed 32-bit integer
    int b = 1;          // Value to cause overflow
    int result;

    // Triggering overflow flag setting through signed addition
    asm volatile(
        "addi r31, r0, %[a]  /* id: l.addi, encoding: 0x27 (l.addi rD, rA, I) */\n"
        "addi r4, r31, %[b]  /* id: l.addi, encoding: 0x27 (l.addi rD, rA, I) */\n"
        "/* example_hex: 0xA0610001 */"
        : "=r" (result)
        : [a] "r" (a), [b] "r" (b)
    );

    return 0;
}
