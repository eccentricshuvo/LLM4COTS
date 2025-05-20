#include <stdint.h>

volatile uint32_t array[4096];

int main(void)
{
    // Initialize array to avoid compiler optimizing out
    for (int i = 0; i < 4096; i++) {
        array[i] = (uint32_t)i;
    }

    uint32_t sum = 0;
    // Access a large array to provoke multiple memory loads leading to cache misses
    for (int i = 0; i < 4096; i++) {
        uint32_t val;
        // Use OpenRISC ISA JSON mnemonics for loads:
        // 'l.lw' is the correct mnemonic for load word (32-bit)
        // Syntax: l.lw rD, offset(rA)
        __asm__ volatile (
            "l.lw %0, 0(%1)\n"    // load word 
            : "=r" (val)
            : "r" (&array[i])
            : 
        );
        sum += val;
    }
    return (int)sum;
}