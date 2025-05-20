int main(void)
{
    volatile int a = 1;
    volatile int b;
    volatile int c;

    /*
     Using OpenRISC-1000 assembler mnemonics as per JSON spec:
     'add' instruction requires three registers, immediate addition uses 'addi'.
     Use 'addi' to add immediate 1.
     Chain dependent adds to provoke operand hazard.
    */
    __asm__ volatile (
        "addi %0, %1, 1\n\t"   /* b = a + 1 */
        "addi %2, %0, 1\n\t"   /* c = b + 1 */
        : "=&r" (b), "+r" (a), "=&r" (c)
        :
        : /* no clobbers */
    );

    return c;
}