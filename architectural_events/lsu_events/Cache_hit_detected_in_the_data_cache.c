#include <stdint.h>

volatile uint32_t buffer[4] = {0x12345678u, 0x9ABCDEF0u, 0x0FEDCBA9u, 0x87654321u};

int main(void)
{
    volatile uint32_t sum = 0;

    /* Warm-up loop to fill cache lines with the buffer's data */
    for (int i = 0; i < 1024; ++i)
    {
        sum += buffer[i & 3]; /* repeatedly access same small set of addresses to get cache hits */
    }

    /* Prevent compiler from optimizing away */
    return (int)sum;
}