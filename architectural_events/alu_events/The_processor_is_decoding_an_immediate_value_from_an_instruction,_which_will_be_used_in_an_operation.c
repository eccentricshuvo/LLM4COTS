int main(void)
{
    volatile int a = 10;
    volatile int b = 5;
    volatile int c;

    /* Pure C code with immediate operands in arithmetic and logical operations */
    c = a + 3;  /* addi-like immediate operation */
    c = b - 2;  /* subi-like immediate operation */
    c = c & 0xF; /* andi-like immediate operation */
    c = c | 0x10; /* ori-like immediate operation */
    c = c ^ 0xA; /* xori-like immediate operation */

    /* Load and store with immediate offsets cannot be done in pure C without variables,
       but the above uses immediate values for operators and should trigger decode_immediate_i */

    (void)c; /* prevent optimization of c */

    return 0;
}