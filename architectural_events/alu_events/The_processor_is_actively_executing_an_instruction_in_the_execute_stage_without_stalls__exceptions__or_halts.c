int main(void)
{
    volatile int a = 1;
    volatile int b = 2;
    volatile int c = 0;
    volatile int mem[4] = {10, 20, 30, 40};
    int i;

    // Loop with a reasonable count to ensure forward progress and timely exit
    for (i = 0; i < 10000; ++i)
    {
        c = a + b;          // simple arithmetic
        c = a - b;          // simple arithmetic
        c = a & b;          // simple logic
        c = a | b;          // simple logic

        mem[i & 3] = c;     // simple store
        c = mem[(i + 1) & 3]; // simple load
    }

    return c;
}