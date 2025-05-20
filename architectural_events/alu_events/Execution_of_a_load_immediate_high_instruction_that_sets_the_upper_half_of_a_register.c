int main(void)
{
    /* Load a 32-bit constant that requires setting the upper half of a register */
    volatile unsigned int x = 0xABCD1234u;

    /* Use variable to prevent optimization */
    return (int)x;
}