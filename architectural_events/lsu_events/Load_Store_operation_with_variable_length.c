int main(void)
{
    volatile unsigned char mem[8] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x07, 0x18};
    volatile unsigned int w;
    volatile unsigned short h;
    volatile unsigned char b;
    volatile unsigned int *wptr = (unsigned int *)mem;
    volatile unsigned short *hptr = (unsigned short *)(mem + 2);
    volatile unsigned char *bptr = mem + 4;

    /* Load word (4 bytes) */
    w = *wptr;

    /* Load halfword (2 bytes) */
    h = *hptr;

    /* Load byte (1 byte) */
    b = *bptr;

    /* Store word */
    *wptr = w;

    /* Store halfword */
    *hptr = h;

    /* Store byte */
    *bptr = b;

    return 0;
}