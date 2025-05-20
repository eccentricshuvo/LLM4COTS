int main(void)
{
    volatile unsigned int a = 0xF0F0F0F0u;
    volatile unsigned int b = 0x0F0F0F0Fu;
    volatile unsigned int c;

    /* Pure C bitwise operations to trigger LUT operations:
       AND, OR, XOR and shifts */
    c = a & b;
    c = a | b;
    c = a ^ b;
    c = a << 4;
    c = b >> 3;

    /* Use the result to avoid optimization away */
    return (int)(c);
}