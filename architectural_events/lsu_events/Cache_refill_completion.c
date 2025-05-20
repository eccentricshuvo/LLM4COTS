#include <stdint.h>

/* 
 * Test program to provoke cache refill_done signal on OpenRISC-1000.
 * Strategy:
 * - Access an array larger than typical cache size to ensure cache misses.
 * - Perform both loads and stores to the array.
 * - Multiple passes exercise cache refill on misses and hits.
 */

#define ARRAY_SIZE 4096
volatile uint32_t array[ARRAY_SIZE];

int main(void)
{
    // Initialize array to known values.
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = (uint32_t)i;
    }

    volatile uint32_t sum = 0;

    // Access pattern to cause cache misses followed by hits.
    for (int pass = 0; pass < 4; ++pass) {
        for (int i = 0; i < ARRAY_SIZE; i += 64) {
            // Read access to cause possible load miss and refill
            sum += array[i];
            // Write access to cause possible store miss and refill
            array[i] = sum + pass;
        }
    }

    // Return value to prevent optimization
    return (int)sum;
}