#include <stdint.h>

int main(void)
{
    /* Trigger carry_in by adding two large uint32_t values causing unsigned overflow */
    volatile uint32_t a = 0xFFFFFFFFu;
    volatile uint32_t b = 1u;
    volatile uint32_t c;

    c = a + b;  /* This addition generates a carry-out from the most significant bit */

    /* Prevent optimizer from removing c */
    return (int)c;
}