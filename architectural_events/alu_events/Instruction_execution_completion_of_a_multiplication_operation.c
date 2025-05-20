int main(void)
{
    int a = 5;
    int b = 7;
    int c = 3;
    int d = 2;
    int result = 1;

    // Multiply a and b
    result = a * b;

    // Chain operations to keep the multiplier busy and produce mul_valid signals
    result = result * c;
    result = result * d;

    // Additional multiplies involving negative values for variety
    result = result * (-a);
    result = result * (-b);

    // Use the result in further multiplies to maintain activity
    for (int i = 1; i <= 10; i++)
    {
        result = result * i;
    }

    // Avoid compiler optimizing away result
    return result;
}