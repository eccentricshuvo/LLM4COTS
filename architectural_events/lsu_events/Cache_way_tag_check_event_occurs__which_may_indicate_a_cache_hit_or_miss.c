#include <stdint.h>

/* Volatile array to ensure memory operations are not optimized away */
volatile uint32_t buffer[64] = {0};

int main(void) {
    /* Increased memory footprint and perform strided accesses to exercise cache way tag check */

    /* Write pattern to buffer with stride 4 to touch multiple cache lines/ways */
    for (int i = 0; i < 64; i += 4) {
        buffer[i] = (uint32_t)(i * 5 + 11);
    }

    /* Read pattern back with different stride to stress cache tag checks */
    uint32_t sum = 0;
    for (int i = 3; i < 64; i += 8) {
        sum += buffer[i];
    }

    /* Small delay loop to ensure enough instructions executed */
    for (volatile int delay = 0; delay < 1000; delay++) {
        asm volatile("or 0,0,0"); /* OR r0,r0,r0 is a valid no-op in OpenRISC */
    }

    /* Use the sum to prevent compiler optimizing away loads */
    return (int)sum;
}