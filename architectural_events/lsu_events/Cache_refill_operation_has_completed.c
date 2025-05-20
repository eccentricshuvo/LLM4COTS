#include <stdint.h>

volatile uint32_t large_array[16384];  // Large array to exceed typical cache size

int main(void)
{
    volatile uint32_t acc = 0;

    // Initialize array to prevent zero-page optimizations
    for (uint32_t i = 0; i < 16384; i++) {
        large_array[i] = i;
    }

    // Inline asm with load word (l.w) using correct OpenRISC syntax
    for (uint32_t i = 0; i < 16384; i++) {
        uint32_t val;
        __asm__ volatile (
            "l.w %0, 0(%1)\n\t"  // l.w is the OpenRISC load word mnemonic
            : "=r"(val)
            : "r"(&large_array[i])
            :
        );
        acc += val;
    }

    // Prevent compiler optimizing away acc
    return (int)acc;
}