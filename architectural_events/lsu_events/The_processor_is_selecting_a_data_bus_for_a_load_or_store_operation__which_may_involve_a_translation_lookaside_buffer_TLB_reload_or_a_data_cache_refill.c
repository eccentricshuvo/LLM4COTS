#include <stdint.h>

/* Large array to attempt TLB reload and data cache refill by accessing many pages */
volatile uint32_t large_array[65536];  /* 65536 * 4 bytes = 256KB, spanning many pages */

int main(void)
{
    volatile uint32_t val = 0;

    /* Initialize array to avoid any unwritten memory optimizations */
    for (uint32_t i = 0; i < 65536; i++) {
        large_array[i] = i;
    }

    /* Access the array with stride 1 to ensure many load and store operations over many pages,
       increasing likelihood of causing TLB reloads or data cache refills */
    for (uint32_t i = 0; i < 65536; i++) {
        val = large_array[i];        /* load */
        large_array[i] = val + 1;    /* store */
    }

    /* Return val to prevent optimization */
    return (int)val;
}