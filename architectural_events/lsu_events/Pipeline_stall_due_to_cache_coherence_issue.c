#include <stdint.h>

volatile int shared = 0;

int main(void) {
    /*
     * To provoke the snoop_check event due to cache coherence in OpenRISC,
     * perform conflicting memory accesses with explicit memory barrier instructions.
     * Use l.lwarx / l.swarx (load-reserved/store-conditional) to generate cache coherence traffic.
     */

    int val;

    /* Load-reserved from shared */
    __asm__ volatile (
        "l.lwarx r3, 0(%1)\n\t"    /* Load-reserved r3 = shared */
        : "=r"(val)
        : "r"(&shared)
        : "r3", "memory"
    );

    /* Store-conditional to shared */
    int sc;
    __asm__ volatile (
        "l.swarx %0, r4, 0(%1)\n\t"
        : "=r"(sc)
        : "r"(&shared), "r"(val + 1)
        : "memory"
    );

    (void)sc;  /* Avoid unused variable warning */

    /* Full memory barrier */
    __asm__ volatile (".word 0x00000107 /* l.sync */" ::: "memory");

    return 0;
}