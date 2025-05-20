int main(void)
{
    // Cause an exception: divide by zero will generate an exception on OpenRISC
    volatile int a = 42;
    volatile int b = 0;
    volatile int c;

    // This will cause an exception
    // The processor trap handler will be invoked.
    // Then we execute the return-from-exception instruction below to return.
    c = a / b;

    // Use raw opcode for "return from exception" (rfe) instruction.
    asm volatile(".word 0x0000C801"); // id: rfe

    // Prevent optimizer from removing the variable use
    return (int)c;
}