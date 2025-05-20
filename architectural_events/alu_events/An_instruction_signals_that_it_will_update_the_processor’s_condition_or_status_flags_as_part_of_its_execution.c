int main(void)
{
    int a = 15;
    int b = 27;
    int c;

    /* Perform arithmetic operations that update condition flags.
     * In OpenRISC, arithmetic instructions (add, sub, etc.) update flags by default.
     * Pure C addition/subtraction here will compile to add/sub instructions that update flags.
     */
    c = a + b;  /* add with flag update */
    c = b - a;  /* sub with flag update */

    /* Logical operations also update flags */
    c = a & b;  /* and */
    c = a | b;  /* or */
    c = a ^ b;  /* xor */

    /* Comparison implemented as subtraction to update flags */
    c = (b > a) ? 1 : 0;

    /* Prevent optimization by volatile pointer */
    volatile int *dummy = &c;
    (void)*dummy;

    return 0;
}