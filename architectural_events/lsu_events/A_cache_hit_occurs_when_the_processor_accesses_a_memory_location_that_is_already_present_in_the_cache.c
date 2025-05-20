#include <stdint.h>

#define ARRAY_SIZE 64

volatile uint32_t array[ARRAY_SIZE];

/* 
 * This test accesses the same memory locations multiple times in a loop,
 * filling the cache and then generating cache hits on subsequent accesses.
 * Pure C is sufficient to cause this event by temporal locality.
 */

int main(void)
{
    uint32_t sum = 0;
    int i, iter;

    /* Initialize array to ensure cache fill on first accesses */
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = (uint32_t)i;
    }

    /* Access the array multiple times to create cache hits */
    for (iter = 0; iter < 1024; iter++) {
        for (i = 0; i < ARRAY_SIZE; i++) {
            /* Use volatile read via inline asm to force load instruction */
            uint32_t val;
            __asm__ volatile (
                "l.lw %[result], %[offset](%[base])" /* load word */
                : [result] "=r"(val)
                : [base] "r"(array), [offset] "I"(i * 4)
                : "memory"
            );
            sum += val;
        }
    }

    /* Use sum to prevent compiler optimizing away the loop */
    return (int)sum;
}