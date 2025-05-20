#include <stdint.h>

/* Volatile memory to force load/store instructions for address calculation */
volatile uint32_t mem[4] = {0x12345678u, 0x9ABCDEF0u, 0x0FEDCBA9u, 0x87654321u};

int main(void) {
    volatile uint32_t a, b;

    /* Load from mem[0] using a register indirect addressing */
    a = mem[0];

    /* Store to mem[1] using register indirect addressing */
    mem[1] = a + 1;

    /* Simple arithmetic instruction with immediate */
    b = a + 42;

    /* Load from mem[2] with displacement */
    a = mem[2];

    /* Store to mem[3] with added displacement */
    mem[3] = b;

    /* Access elements to generate different instruction addresses */
    for (int i = 0; i < 4; ++i)
        b += mem[i];

    /* Inline asm using OpenRISC mnemonics from JSON header */
    register uint32_t val asm("r3");
    register uint32_t addr asm("r4") = (uint32_t)&mem[1];

    asm volatile(
        "l.lwz %0,0(%1)\n\t"      /* lwz: load word with zero extension (lw) */
        "l.addi %0,%0,10\n\t"     /* addi: add immediate */
        "l.sw %0,4(%1)\n\t"       /* sw: store word */
        : "=r" (val)
        : "r" (addr)
        : "memory"
    );

    return (int)(b + val);
}