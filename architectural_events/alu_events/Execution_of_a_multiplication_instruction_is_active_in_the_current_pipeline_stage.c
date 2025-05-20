int main(void)
{
    volatile int a = 12345;
    volatile int b = -6789;
    volatile int c = 0;
    volatile int d = 0x7FFFFFFF;
    volatile int e = -1;

    // Perform signed multiplications with varying operands to activate multiply execution
    c = a * b;    // signed multiply negative
    c = a * c;    // signed multiply positive * possibly negative
    c = b * e;    // signed multiply negative * -1
    c = d * a;    // large positive value multiply
    c = c * 0;    // multiply by zero

    // Prevent optimization by using volatile variables and dependencies
    return c;
}