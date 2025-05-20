int main(void)
{
    volatile int a = 0x7FFFFFFF;  // max positive 32-bit int
    volatile int b = 1;
    volatile int result;

    // This addition causes signed overflow (carry out) to test adder_result_o
    result = a + b;

    // Also test subtraction underflow
    result = (-a) - b;

    // Keep result volatile to prevent optimization
    return (int)result;
}