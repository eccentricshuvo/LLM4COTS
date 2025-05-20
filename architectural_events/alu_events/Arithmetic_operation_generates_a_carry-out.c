int main(void)
{
    unsigned int a = 0xFFFFFFFFu;
    unsigned int b = 1u;
    unsigned int result = a + b;

    /* avoid optimization away */
    return (int)result;
}