int main() {
    // Triggering exception handling mechanisms with an instruction
    asm volatile(
        "l.xor r0, r0, r0  /* id: l.xor, encoding: 00000000000000000000000000000000 (0x00000000) */" // XOR with zero to trigger a fault exception
        "/* No example_hex available */"
    );

    return 0;
}