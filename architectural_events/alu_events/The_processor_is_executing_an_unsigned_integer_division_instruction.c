int main(void)
{
    unsigned int numerator = 100u;
    unsigned int denominator = 3u;
    unsigned int quotient;

    quotient = numerator / denominator;

    /* Use quotient to prevent optimization */
    return (int)quotient;
}