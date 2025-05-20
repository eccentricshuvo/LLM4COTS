int main(void)
{
    volatile int a = 1234;
    volatile int b = 5678;
    volatile int c;

    /* Perform arithmetic operations */
    c = a + b;    /* addition triggers ALU */
    c = a - b;    /* subtraction triggers ALU */
    c = a & b;    /* bitwise and triggers ALU */
    c = a | b;    /* bitwise or triggers ALU */
    c = a ^ b;    /* bitwise xor triggers ALU */
    c = (a < b);  /* comparison triggers ALU (set if less) */

    /* Prevent optimization by using volatile */
    return c;
}