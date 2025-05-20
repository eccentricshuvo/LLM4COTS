#include <stdint.h>

int main(void)
{
    uint32_t u = 0x12345678u;
    int32_t s = -16;

    /* Logical left shift by 4 */
    uint32_t u_shift_left = u << 4;

    /* Logical right shift by 4 */
    uint32_t u_shift_right = u >> 4;

    /* Arithmetic right shift by 4 */
    int32_t s_shift_right = s >> 4;

    /* Shift by zero (no shift) */
    uint32_t u_shift_zero = u << 0;

    /* Shift by maximum bit width minus one (31 for 32-bit) */
    uint32_t u_shift_max = u << 31;

    /* Use volatile to prevent optimization */
    volatile uint32_t vv1 = u_shift_left;
    volatile uint32_t vv2 = u_shift_right;
    volatile int32_t vv3 = s_shift_right;
    volatile uint32_t vv4 = u_shift_zero;
    volatile uint32_t vv5 = u_shift_max;

    /* Avoid optimized away */
    return (int)(vv1 + vv2 + vv3 + vv4 + vv5);
}