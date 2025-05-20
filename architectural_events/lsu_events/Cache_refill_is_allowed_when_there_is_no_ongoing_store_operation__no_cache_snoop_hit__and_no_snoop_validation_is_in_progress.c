#include <stdint.h>

/* 
 * This test aims to provoke cache refill operations when allowed by
 * the hardware, i.e., no ongoing store, no snoop hits, and no snoop validation.
 * Strategy:
 *  - Perform multiple load accesses to distinct memory addresses likely missing in cache,
 *    to cause cache refill.
 *  - Avoid stores to ensure no ongoing store operation.
 *  - Volatile prevents compiler optimization of loads.
 */

volatile uint32_t mem[256];

int main(void)
{
    uint32_t acc = 0;
    /* Touch some memory to prevent fully empty cache state */
    for (int i = 0; i < 256; i += 16) {
        acc += mem[i];
    }

    /* Perform multiple loads from different cache lines to cause cache misses/refills */
    for (int i = 0; i < 256; i += 8) {
        acc += mem[i];
    }

    /* Use acc to prevent optimization */
    return (int)acc;
}