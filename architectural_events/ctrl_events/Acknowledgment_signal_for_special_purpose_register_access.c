// C test program to trigger the acknowledgment signal for special purpose register access
#include <stdio.h>

int main() {
    // Triggering acknowledgment signal for special purpose register access
    asm volatile(
        "l.mfspr r5, r0, 8  /* id: l.mfspr, encoding: 1000b (l.mfspr rD, rA, SPR) */" // Reading from Performance Counters Configuration register (PCCFGR)
        "/* example_hex: 0xDEADBEEF */"
        :::"r5"
    );
    
    return 0;
}