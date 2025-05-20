int main(void)
{
    volatile int a = 15;
    volatile int b = 27;
    volatile int c = 0;

    /* Arithmetic operations */
    c = a + b;
    c = a - b;
    c = a * b;
    c = a / (b ? b : 1); /* Prevent division by zero */

    /* Logical operations */
    c = a & b;
    c = a | b;
    c = a ^ b;

    /* Branch-like behavior using conditional move equivalent in pure C */
    if (a == b) {
        c = 1;
    } else if (a != b) {
        c = 2;
    } else if (a < b) {
        c = 3;
    } else if (a > b) {
        c = 4;
    }

    /* Move operations (register or memory load/store semantics in C) */
    volatile int d = c;
    c = d;

    return c;
}