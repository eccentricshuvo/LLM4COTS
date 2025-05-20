int main(void)
{
    volatile float a = 1.5f;
    volatile float b = 2.5f;
    volatile float c = 0.0f; // initialize c to avoid use of uninitialized variable

    // Perform floating-point arithmetic operations multiple times to trigger fpu_arith_valid event
    // Loop to ensure several FPU operations complete and the event triggers
    for (int i = 0; i < 1000; i++) {
        c = a + b;
        c = c - a;
        c = c * b;
        c = c / a;
        // Add small variation to prevent optimization of loop away
        a = c - b;
        b = c - a;
    }

    // Use the result to prevent optimization away
    return (int)(c * 10.0f);
}