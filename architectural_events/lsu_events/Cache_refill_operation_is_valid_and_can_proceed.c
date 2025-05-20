#include <stdint.h>

#define ARRAY_SIZE (65536)  /* Increased further to ensure cache misses and trigger refill_valid */

volatile uint32_t array[ARRAY_SIZE];

int main(void)
{
    /* Initialize array with some values */
    for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    volatile uint32_t sum = 0;

    /* Loop to access and modify array to cause cache refill operations */
    for (uint32_t iter = 0; iter < 128; iter++) {
        for (uint32_t i = 0; i < ARRAY_SIZE; i += 2) {
            sum += array[i];
        }
        for (uint32_t i = 0; i < ARRAY_SIZE; i += 4) {
            array[i] = sum + i;
        }
    }

    /* Return sum to prevent optimization */
    return (int)sum;
}