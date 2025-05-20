int main(void)
{
    unsigned int a = 5;
    unsigned int b = 10;
    int result;

    // Pure C less-than comparison to trigger a_ltu_b signal condition internally
    // The hardware signal would result from this comparison in the ALU.
    result = (a < b);

    // Use the result to avoid optimization away
    return result;
}