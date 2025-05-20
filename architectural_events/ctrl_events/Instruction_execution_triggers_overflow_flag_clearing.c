int main() {
    // Triggering overflow flag clearing during instruction execution
    asm volatile(
        "l.ori r0, r0, 0  /* id: l.ori, encoding: 0110111 (l.ori rD, rA, imm) */" // Perform logical OR with immediate value 0 to clear overflow flag
        "/* example_hex: 0xDEAD10CC */"
        ::: "r0"
    );

    return 0;
}