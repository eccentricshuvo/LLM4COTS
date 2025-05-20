// C test program to trigger operations related to special purpose registers
int main() {
    // Using inline assembly to trigger special purpose register operations
    asm volatile(
        "l.mfspr r5, r0, 8  /* id: l.mfspr, encoding: 1000b (l.mfspr rD, rA, SPR) */" // Reading from Performance Counters Configuration register (PCCFGR)
        "/* example_hex: 0xDEADBEEF */"
        :::"r5"
    );

    return 0;
}