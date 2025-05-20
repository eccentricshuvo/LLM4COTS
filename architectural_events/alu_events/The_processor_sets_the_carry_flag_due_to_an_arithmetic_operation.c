int main(void)
{
    unsigned int a = 0xFFFFFFFFu;
    unsigned int b = 1u;
    unsigned int c = a + b;  // This addition will overflow and set the carry flag.

    // Prevent optimization away
    return (int)c;
}