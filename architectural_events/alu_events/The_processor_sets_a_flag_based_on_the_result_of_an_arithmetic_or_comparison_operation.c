int main(void)
{
    volatile int a = 10;
    volatile int b = 20;
    volatile int c;

    // Arithmetic operation - addition
    c = a + b;   // Sets flags based on addition result (e.g., zero, carry if applicable)

    // Comparison operation - less than
    if (a < b)
    {
        c = 1;  // Flags affected by comparison
    }
    else
    {
        c = 0;
    }

    // Logical operation - AND
    c = a & b;   // Flags may be set based on result being zero or non-zero

    // Use volatile to prevent optimization
    return c;
}