int main(void)
{
    int x = 5;
    int y = 42;
    int condition = 1;  /* true condition */

    /* Force inline assembly with conditional move using OpenRISC ISA mnemonic */
    /* or minimal inline asm if compiler supports it. Assuming OP cmov opcode 0xE exists,
       use a direct minimal inline asm */

    /* Unfortunately, OpenRISC conditional move instructions use 'cmov' with conditions,
       but C compiler optimization may not emit it unless explicit asm given.
       This asm moves y into x if condition is nonzero, else leaves x unchanged. */

    __asm__ volatile (
        "cmov %1, %2, %0\n\t"  /* cmov y, condition, x */
        : "+r"(x)
        : "r"(y), "r"(condition)
        :
    );

    /* Prevent optimizing away */
    return x;
}