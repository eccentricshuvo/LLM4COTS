int main(void)
{
    volatile int a = 100;
    volatile int b = 5;
    volatile int c;

    // Perform division to trigger div_valid signal
    c = a / b;

    // Use the result to prevent optimization
    if (c == 20)
    {
        // Do nothing
    }
    return 0;
}