/* 
 * Test program to provoke a cache line replacement caused by the LRU policy 
 * in the OpenRISC 1000 cache.
 *
 * Strategy:
 * 1. Use a set of volatile arrays spaced to ensure they map to the same cache set but different ways.
 * 2. Access each array's first element sequentially to fill the cache ways for one set.
 * 3. Access a new memory location that maps to the same set, causing an LRU replacement.
 * 4. Use volatile to avoid optimization and force actual memory accesses.
 *
 * This pure C code should trigger data cache line replacements, influenced by LRU.
 */

#define CACHE_WAYS 4       // Assume typical 4-way set associative cache
#define CACHE_LINE_SIZE 32 // Typical cache line size in bytes
#define ARRAY_SIZE 64      // Ensure arrays are large enough to occupy different cache lines

/* 
 * Place arrays sufficiently far to map onto the same cache set.
 * Assuming cache set-indexing from address bits: 
 * We'll space arrays by multiple of (number of sets * cache line size).
 * We conservatively space by CACHE_LINE_SIZE*CACHE_WAYS to cause conflict.
 */

volatile char arr0[ARRAY_SIZE * CACHE_LINE_SIZE];
volatile char arr1[ARRAY_SIZE * CACHE_LINE_SIZE];
volatile char arr2[ARRAY_SIZE * CACHE_LINE_SIZE];
volatile char arr3[ARRAY_SIZE * CACHE_LINE_SIZE];
volatile char arr4[ARRAY_SIZE * CACHE_LINE_SIZE]; // Accessing this will cause eviction

int main(void)
{
    /* Fill the cache set by touching one byte in each way */
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        volatile char x0 = arr0[i * CACHE_LINE_SIZE];
        volatile char x1 = arr1[i * CACHE_LINE_SIZE];
        volatile char x2 = arr2[i * CACHE_LINE_SIZE];
        volatile char x3 = arr3[i * CACHE_LINE_SIZE];

        /* Use values to prevent optimization */
        if (x0 + x1 + x2 + x3 == 0xFF)
            return 1; // Dummy condition never true
    }

    /* Access a new line that maps to the same cache set to cause LRU replacement */
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        volatile char y = arr4[i * CACHE_LINE_SIZE];

        /* Dummy use */
        if (y == 0xFF)
            return 2; // Never true
    }

    return 0;
}