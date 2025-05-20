int main(void)
{
    volatile int x = 10;
    volatile int y;

    /* Use immediate values in arithmetic and logical operations */
    y = x + 5;    /* addi instruction in C */
    y = y - 3;    /* subi instruction in C */
    y = y & 0xFF; /* andi instruction in C */
    y = y | 0x0F; /* ori instruction in C */

    /* Use immediate in comparison */
    if (y == 42)
    {
        y = 1;
    }
    else
    {
        y = 0;
    }

    /* Use immediate offset in load/store style C */
    volatile int arr[4] = {1, 2, 3, 4};
    y = arr[1];   /* load with immediate offset */
    arr[2] = y;   /* store with immediate offset */

    return 0;
}