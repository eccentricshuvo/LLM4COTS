int main(void)
{
    volatile unsigned int a, b, c;

    a = 0xFFFFFFFFu;
    b = 1u;

    // Addition that causes overflow, generating a carry-out
    c = a + b;

    // Subtraction that causes borrow (carry flag set)
    c = 0u - 1u;

    // Multiplication - might affect carry flag depending on implementation
    c = a * b;

    // Division (no carry flag typically affected, but test anyway)
    c = a / b;

    // Prevent optimizer removing the above computations
    return (int)c;
}