int main(void)
{
    /*
     * Insert a deliberate exception trigger followed by the return-from-exception instruction.
     * This will reliably provoke the doing_rfe_r signal as the processor handles the exception and returns.
     */

    volatile int a = 1;
    volatile int b = 0;
    volatile int c;

    // Trigger division by zero exception
    c = a / b;

    // Return from exception instruction encoded as .word 0x40
    asm volatile (".word 0x40" ::: "memory");

    return 0;
}