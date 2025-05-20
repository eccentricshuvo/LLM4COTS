int main(void)
{
    volatile int a = 1234;
    volatile int b = -5678;
    volatile int c = 910;

    volatile int res1 = a * c;    // signed multiplication
    volatile unsigned int ua = 1234u;
    volatile unsigned int uc = 910u;
    volatile unsigned int res2 = ua * uc;  // unsigned multiplication

    // Perform multiple multiplications in a loop to ensure event triggers
    for (volatile int i = 1; i < 100; i++) {
        res1 += b * i;   // signed multiplication
        res2 += ua * (uc + i);  // unsigned multiplication
    }

    // Use the results to prevent optimization
    if (res1 == 0 || res2 == 0)
        return 1;

    return 0;
}