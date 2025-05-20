int main(void)
{
    volatile int a = 0x55AA55AA;
    volatile int b = 0x0F0F0F0F;
    volatile int c = 0x33333333;
    volatile int d = 0xAAAAAAAA;
    volatile int e = 0x12345678;
    volatile int f = 0x87654321;
    volatile int res;

    // Arithmetic operations
    res = a + b;       // add
    res = a - b;       // subtract

    // Logical operations
    res = a & b;       // AND
    res = a | b;       // OR
    res = a ^ b;       // XOR
    res = ~a;          // NOT

    // Shift operations
    res = a << 4;      // logical left shift
    res = a >> 4;      // logical right shift (for unsigned would be logical, here signed: arithmetic)
    res = (unsigned int)a >> 4; // logical right shift forced

    // Comparison - set less than (simulate with conditional expression)
    res = (a < b) ? 1 : 0;

    // Move/immediate via addition with 0
    res = e + 0;

    // Use the volatile res to prevent optimization away
    return res;
}