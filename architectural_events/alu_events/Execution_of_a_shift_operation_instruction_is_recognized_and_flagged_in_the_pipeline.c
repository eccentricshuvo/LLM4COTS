int main(void)
{
    unsigned int x = 0xF0F0F0F0u;

    /* Shift left by 0 (no-op) */
    unsigned int y0 = x << 0;

    /* Shift left by 1 */
    unsigned int y1 = x << 1;

    /* Shift right logical by 4 */
    unsigned int y2 = x >> 4;

    /* Shift right arithmetic by 4, using signed int */
    int sx = (int)x;
    int y3 = sx >> 4;

    /* Shift left by maximum typical 31 bits (assuming 32-bit int) */
    unsigned int y4 = x << 31;

    /* Shift right logical by maximum 31 bits */
    unsigned int y5 = x >> 31;

    /* Dummy use to avoid optimization */
    volatile unsigned int dummy = y0 + y1 + y2 + (unsigned int)y3 + y4 + y5;

    (void)dummy;

    return 0;
}