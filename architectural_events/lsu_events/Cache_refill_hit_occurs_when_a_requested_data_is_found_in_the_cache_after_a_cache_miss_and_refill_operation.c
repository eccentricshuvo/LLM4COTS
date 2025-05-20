/* 
 * Test program to trigger the "refill_hit" event in OpenRISC 1000.
 * 
 * Strategy:
 * 1. Load data from a memory address to populate (refill) the cache.
 * 2. Access a different address causing a cache miss.
 * 3. Access the first address again, which should be a refill hit.
 *
 * We use volatile memory access to prevent optimization that could eliminate necessary loads/stores.
 * Pure C loads/stores trigger cache operations as per the design.
 */

volatile unsigned int buffer[64] __attribute__((aligned(64)));

int main(void)
{
    volatile unsigned int tmp;
    unsigned int i;

    /* Step 1: Initialize buffer with distinct values */
    for (i = 0; i < 64; i++) {
        buffer[i] = i;
    }

    /* Step 2: Access first element to load it into cache (refill) */
    tmp = buffer[0];

    /* Step 3: Access a distant element to cause a cache miss and refill */
    tmp = buffer[32];

    /* Step 4: Access first element again - this triggers a refill_hit as it should now be in cache */
    tmp = buffer[0];

    /* Avoid optimization */
    (void)tmp;

    return 0;
}