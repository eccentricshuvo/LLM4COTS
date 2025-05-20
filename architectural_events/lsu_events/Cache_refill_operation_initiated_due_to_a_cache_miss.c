#include <stdint.h>

/* Large array to ensure cache miss */
#define ARRAY_SIZE 16384
volatile uint32_t large_array[ARRAY_SIZE];

/* Prevent optimization */
volatile uint32_t sink;

int main(void)
{
    /* Touch array with stride to provoke cache misses */
    for (uint32_t i = 0; i < ARRAY_SIZE; i += 16) {
        uint32_t val;
        /* OpenRISC lw syntax: lw rD, imm(rA) */
        __asm__ volatile(
            "lw %0, 0(%1)\n"
            : "=r" (val)
            : "r" (&large_array[i])
        );
        sink = val; /* Prevent optimizing away the load */
    }

    return 0;
}