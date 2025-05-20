#include <stdint.h>

#define ARRAY_SIZE 4096

/* Large array to force data cache misses during iteration */
volatile uint32_t large_array[ARRAY_SIZE];

int main(void)
{
    /* Initialize array with some values */
    for (uint32_t i = 0; i < ARRAY_SIZE; ++i)
        large_array[i] = i;

    uint32_t sum = 0;

    /*
     * Access elements spaced apart by 64 bytes (16 uint32_t words)
     * This non-contiguous access pattern increases the chance of data cache misses,
     * provoking the data cache refill burst operation (dbus_burst_o).
     */
    for (uint32_t i = 0; i < ARRAY_SIZE; i += 16)
        sum += large_array[i];

    /* Avoid optimization by using sum */
    return (int)sum;
}