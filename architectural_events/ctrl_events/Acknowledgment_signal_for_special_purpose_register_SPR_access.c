int main() {
    // Triggering acknowledgment signal for special purpose register access
    asm volatile(
        "l.mfspr r5, r0, 8  /* id: l.mfspr, encoding: 0x2d (l.mfspr rD,rA,K) */" // Reading from Performance Counters Configuration register (PCCFGR)
        "/* example_hex: 0xB4000000 */"
        :::"r5"
    );

    return 0;
}