int main(void)
{
    volatile int a = 1, b = 2, c = 3, d = 4, e = 5;
    volatile int r1, r2, r3, r4, r5;
    volatile int r6, r7;

    // Independent arithmetic and bitwise to avoid stalls
    r1 = a + b;    // add
    r2 = c - d;    // sub
    r3 = e << 1;   // shift left
    r4 = a | b;    // or
    r5 = c & d;    // and

    // Add multiplication to engage mul unit (assuming mul unit readiness is needed)
    r6 = a * b;    // mul

    // Add division to engage div unit and wait, but do not stall
    r7 = b / a;    // div

    // Prevent compiler optimizing out
    return r1 + r2 + r3 + r4 + r5 + r6 + r7;
}