int main(void)
{
    volatile int a = 10;
    volatile int b = 20;
    volatile int c;

    /* Arithmetic instruction: triggers CPU internal state write (add) */
    c = a + b;

    /* Load and store instructions: triggers CPU internal state write */
    volatile int mem[2] = {1, 2};
    c = mem[0];
    mem[1] = c;

    /* Branch instructions: use conditional to enforce branch */
    if (a != b) {
        c = a - b;
    } else {
        c = b - a;
    }

    /* Exception generating instruction: divide by zero to trigger exception */
    volatile int d = 0;
    /* Avoid compiler optimizing out */
    if (d != 0)
        c = a / d;
    else
        c = a;  /* No operation if divide by zero would happen */

    /* Flag manipulating instructions and context switching instructions 
       are typically not expressible in pure C without inline asm or OS support, 
       so omitted as impossible to express here */

    return c;
}