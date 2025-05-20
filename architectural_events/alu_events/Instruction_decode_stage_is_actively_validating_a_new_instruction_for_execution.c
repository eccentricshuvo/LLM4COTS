int main(void)
{
    volatile int a = 1;
    volatile int b = 2;
    volatile int c = 0;
    volatile int d = 3;
    volatile int e = 4;
    volatile int f = 5;
    volatile int g = 0;

    // Simple sequence of arithmetic instructions to keep pipeline flowing
    // No branches, no stalls, no exceptions.

    for (int i = 0; i < 1000; ++i)
    {
        c = a + b;
        g = c * d;
        a = g - e;
        b = a + f;
        d = b ^ c;
        e = d | f;
        f = e & a;
    }

    // Prevent optimization away
    return g;
}