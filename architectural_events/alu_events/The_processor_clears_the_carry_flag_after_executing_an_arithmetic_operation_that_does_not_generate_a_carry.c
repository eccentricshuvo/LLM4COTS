int main(void)
{
    /* Add two numbers that do not cause a carry out */
    unsigned int a = 1000u;
    unsigned int b = 2000u;
    unsigned int c;

    c = a + b;  /* This addition does not generate carry; carry flag cleared */

    /* Multiply two small numbers to avoid overflow and carry */
    c = a * 2u;

    /* Divide two non-zero, fitting values */
    c = b / 2u;

    /* Use the results somehow to prevent optimization */
    return (int)(c);
}