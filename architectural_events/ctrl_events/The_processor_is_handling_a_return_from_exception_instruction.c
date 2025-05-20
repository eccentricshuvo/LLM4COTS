int main(void)
{
    /*
     * To reliably trigger the 'doing_rfe_o' event, we must cause an exception,
     * then execute the RFE instruction to return from the exception handler.
     * The previous code only executed RFE, but without an exception, it may not assert the signal.
     *
     * Here, div-by-zero triggers an exception.
     * The volatile variable and inline asm ensure the divide is performed and not optimized out.
     * After the exception is taken and handled, executing RFE (.word 0x0000000B) returns from it,
     * which triggers the 'doing_rfe_o' signal.
     */

    volatile int a = 1;
    volatile int b = 0;
    volatile int c;

    // Trigger division by zero exception
    // Prevent compiler from optimizing by using volatile and inline asm barriers
    __asm__ volatile("" ::: "memory");

    c = a / b;

    __asm__ volatile("" ::: "memory");

    // Return from exception instruction
    __asm__ volatile(".word 0x0000000B /* id: rfe, encoding: 00000000000000000000000000001011 */");

    return 0;
}