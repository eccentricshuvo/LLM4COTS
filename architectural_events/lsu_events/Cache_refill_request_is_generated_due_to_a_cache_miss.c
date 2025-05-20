#include <stdint.h>

/*
 * Test program to provoke a dc_refill_req (cache refill request)
 * by loading from a large array that likely exceeds the data cache size.
 * Such accesses to uncached or cold memory lines trigger cache misses,
 * causing the cache refill request signal.
 */

#define ARRAY_SIZE (1024 * 64) /* Large size likely bigger than typical L1 D-cache */

volatile uint32_t large_array[ARRAY_SIZE];

int main(void)
{
    volatile uint32_t acc = 0;

    /* Initialize array to avoid compiler optimizing stores away */
    for (unsigned i = 0; i < ARRAY_SIZE; i++) {
        large_array[i] = (uint32_t)i;
    }

    /*
     * Use inline assembly to perform explicit load instructions
     * that map to OpenRISC 'l.lwz' instruction to ensure cache miss triggers.
     * We load from large_array locations spaced to exceed cache size.
     */
    for (unsigned i = 0; i < ARRAY_SIZE; i += 64) {
        uint32_t val;
        __asm__ volatile (
            "l.lwz %0, %1(%2)"     "\n\t"  /* load word from memory, trigger cache refill if miss */
            : "=r" (val)
            : "r" (i * 4), "r" (large_array)
            : "memory"
        );
        acc += val;
    }

    /* Prevent compiler optimizing acc away */
    return (int)acc;
}