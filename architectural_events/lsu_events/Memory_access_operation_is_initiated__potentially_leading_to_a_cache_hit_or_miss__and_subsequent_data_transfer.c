#include <stdint.h>

volatile uint32_t data[16] = {0};

int main(void)
{
    /* Perform a sequence of loads and stores to provoke memory access signals */
    for (int i = 0; i < 16; i++) {
        data[i] = i * 3;         /* Store operation */
    }

    uint32_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += data[i];          /* Load operation */
    }

    /* Prevent compiler optimizing away */
    return (int)sum;
}