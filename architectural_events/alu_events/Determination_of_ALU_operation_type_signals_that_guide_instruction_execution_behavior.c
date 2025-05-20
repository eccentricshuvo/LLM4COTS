int main(void)
{
    volatile unsigned int x = 0xF0F0F0F0u;
    volatile unsigned int y = 0x0F0F0F0Fu;
    volatile unsigned int z = 0;

    /* ALU logical operations in pure C to exercise different ALU op_logic signals: */

    /* AND operation */
    z = x & y;

    /* OR operation */
    z = x | y;

    /* XOR operation */
    z = x ^ y;

    /* NOT operation */
    z = ~x;

    /* ALU arithmetic operations */

    /* ADD operation */
    z = x + y;

    /* SUBTRACT operation */
    z = x - y;

    /* Special-purpose register move instructions usually require inline asm for or1k, but as per instructions avoid inline asm unless necessary */

    return (int)z;
}