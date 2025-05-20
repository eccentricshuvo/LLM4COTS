int main(void)
{
    volatile int a = -3;
    volatile int b = 7;
    volatile int c;

    c = a * b;

    /* Prevent optimization away */
    return c;
}