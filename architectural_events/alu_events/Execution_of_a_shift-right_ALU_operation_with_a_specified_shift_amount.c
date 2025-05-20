int main(void)
{
    volatile unsigned int x = 0xF0F0F0F0u;
    volatile unsigned int shift;
    volatile unsigned int result = 0;

    /* Various shift amounts: zero, small, boundary, large */
    for (shift = 0; shift <= 40; shift += 8) {
        /* Right shift with varying amounts to trigger opc_alu_shr operand logic */
        result ^= (x >> shift);
    }

    /* Use volatile to avoid optimization away */
    return (int)result;
}