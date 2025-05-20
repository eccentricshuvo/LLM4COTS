#include <stdint.h>

/* The goal is to trigger pipeline stalls caused by data bus acknowledgements (dbus_ack).
 * This is commonly due to memory operations accessing uncached memory.
 * Using volatile pointers to large arrays to force loads and stores,
 * ensuring accesses are not optimized away and likely miss cache.
 */

#define ARRAY_SIZE 4096

volatile uint32_t array1[ARRAY_SIZE];
volatile uint32_t array2[ARRAY_SIZE];

/* OpenRISC-1000 assembler mnemonics:
 * lw  - load word
 * sw  - store word
 * movhi - move high immediate
 * ori - OR immediate
 */

int main(void)
{
    volatile uint32_t temp = 0;

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array1[i] = (uint32_t)i;
        array2[i] = (uint32_t)(ARRAY_SIZE - i);
    }

    for (int iter = 0; iter < 1024; ++iter) {
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            uint32_t val;
            void *src = (void *)&array1[i];
            void *dst = (void *)&array2[i];

            /* Load src pointer into r3 using movhi/ori pair for 32-bit address */
            unsigned int src_hi = ((unsigned int)src) >> 16;
            unsigned int src_lo = ((unsigned int)src) & 0xFFFF;

            unsigned int dst_hi = ((unsigned int)dst) >> 16;
            unsigned int dst_lo = ((unsigned int)dst) & 0xFFFF;

            __asm__ volatile (
                "movhi r3, %1\n\t"
                "ori   r3, r3, %2\n\t"
                "lw    %0, 0(r3)\n\t"
                : "=r"(val)
                : "i"(src_hi), "i"(src_lo)
                : "r3"
            );

            val += iter;

            __asm__ volatile (
                "movhi r4, %1\n\t"
                "ori   r4, r4, %2\n\t"
                "sw    %0, 0(r4)\n\t"
                :
                : "r"(val), "i"(dst_hi), "i"(dst_lo)
                : "r4"
            );

            temp = val;
        }
    }

    return (int)temp;
}