#include <stdint.h>

volatile uint32_t buffer[32];

int main(void)
{
    /* Initialize buffer to avoid unexpected optimizations */
    for (int i = 0; i < 32; ++i)
        buffer[i] = i;

    /* Use OpenRISC 'lwz' and 'stw' mnemonics exactly as listed in the ISA JSON (without 'l.' prefix) */

    for (int i = 0; i < 4096; ++i)
    {
        uint32_t val;
        uint32_t idx = i & 31;
        uint32_t idx_next = (i + 1) & 31;

        /* Load word zero (lwz) */
        __asm__ volatile (
            "lwz %0, 0(%1)  # Load word zero from buffer[idx]\n"
            : "=r"(val)
            : "r"(&buffer[idx])
        );

        val += i;

        /* Store word (stw) */
        __asm__ volatile (
            "stw %0, 0(%1)  # Store word to buffer[idx_next]\n"
            :
            : "r"(val), "r"(&buffer[idx_next])
        );
    }

    /* Return some value to avoid optimization */
    return (int)buffer[0];
}