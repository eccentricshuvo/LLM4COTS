/* Test program to trigger pipeline stall due to cache miss (refill) on OpenRISC-1000.
 *
 * Strategy:
 * - Access a large array exceeding typical cache size.
 * - Access array elements with large stride to reduce spatial locality.
 * - Use volatile to prevent optimization away.
 *
 * This will cause frequent cache misses, provoking refill and pipeline stalls.
 */

#include <stdint.h>

#define ARRAY_SIZE (1024 * 64)  /* Size large enough to exceed typical L1 cache */
#define STRIDE 128             /* Large stride to minimize cache line reuse */

volatile uint32_t large_array[ARRAY_SIZE];

int main(void)
{
    volatile uint32_t sink = 0;
    uint32_t i;

    /* Initialize array with some data */
    for (i = 0; i < ARRAY_SIZE; ++i)
        large_array[i] = i;

    /* Access with large stride to cause cache misses and stalls */
    for (i = 0; i < ARRAY_SIZE; i += STRIDE)
    {
        /* Use inline assembly load word explicitly with OpenRISC mnemonics */
        uint32_t val;
        register uint32_t *ptr_reg asm ("r3") = (uint32_t *)&large_array[i];
        asm volatile (
            "lw %0,0(%1)\n\t"  /* lw rd, 0(rs) */
            : "=r"(val)
            : "r"(ptr_reg)
        );
        sink += val;
    }

    /* Return sink to prevent optimization */
    return (int)sink;
}