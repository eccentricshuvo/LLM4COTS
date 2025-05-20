// C test program to trigger the operations related to special-purpose registers
int main() {
    // Using inline assembly to trigger operations related to special-purpose registers
    asm volatile(
        "l.mtspr r0, r1, 8  /* id: l.mtspr, encoding: 11000b (l.mtspr rS, rA, SPR) */" // Writing to Performance Counters Configuration register (PCCFGR)
        "/* example_hex: 0xDEADBEEF */"
        :::"r0", "r1"
    );

    return 0;
}