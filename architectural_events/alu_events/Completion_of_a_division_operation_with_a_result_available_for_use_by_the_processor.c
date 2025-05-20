int main(void)
{
    volatile int dividend = 100;
    volatile int divisor = 4;
    volatile int result;
    volatile int use_result;

    result = dividend / divisor; /* Integer division triggers div_result */

    /* Use the division result to ensure it is ready and used */
    use_result = result + 10;

    /* Prevent optimization away */
    return use_result;
}