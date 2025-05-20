#include <stdint.h>

/* 
 * Volatile array to avoid compiler optimizations.
 * Large enough to exceed any small data cache size and cause cache refill requests.
 */
volatile uint32_t large_array[1024];

int main(void)
{
    volatile uint32_t sum = 0;

    /* Access elements in a pattern to trigger cache misses and hence refill_req_o */
    for (int i = 0; i < 1024; i += 16)
    {
        /* Load from large_array[i] to provoke cache line refill */
        sum += large_array[i];
    }

    /* Access a different range to force other cache lines to be loaded */
    for (int i = 8; i < 1024; i += 16)
    {
        sum += large_array[i];
    }

    /* Write to large_array in sparse pattern to invalidate some lines and provoke refill on next reads */
    for (int i = 0; i < 1024; i += 32)
    {
        large_array[i] = (uint32_t)(sum + i);
    }

    /* Read again overlapping previous stores, causing cache refill due to invalidation */
    for (int i = 0; i < 1024; i += 16)
    {
        sum += large_array[i];
    }

    return (int)sum;
}