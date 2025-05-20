#include <stdint.h>

int main(void)
{
    /* Use pure C unsigned addition to provoke unsigned overflow.
       Add two large unsigned integers close to UINT32_MAX.
     */
    volatile uint32_t a = 0xFFFFFFFFU;
    volatile uint32_t b = 0x00000001U;
    volatile uint32_t c = a + b;   /* This provokes unsigned overflow */

    /* Prevent optimization by using volatile variables and returning something dependent on c */
    return (int)c;
}