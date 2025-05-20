int main(void)
{
    /* Signed addition without overflow */
    int a = 1000;
    int b = 2000;
    int c = a + b;       /* No overflow */

    /* Signed multiplication without overflow */
    int x = 10;
    int y = 20;
    int z = x * y;       /* No overflow */

    /* Signed division without division-by-zero */
    int dividend = 123456;
    int divisor = 123;
    int quotient = dividend / divisor;  /* Valid division */

    /* Prevent optimizer from removing code */
    return c + z + quotient;
}