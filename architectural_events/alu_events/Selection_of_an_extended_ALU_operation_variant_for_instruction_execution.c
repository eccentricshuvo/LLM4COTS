int main(void)
{
    int a = 15, b = 7;
    int c;

    /* Pure ISO C arithmetic with condition flags triggers extended ALU ops */
    c = a + b;          /* addition */
    c = a - b;          /* subtraction */
    c = a ^ b;          /* xor */
    c = a & b;          /* and */
    c = a | b;          /* or  */
    c = a << 2;         /* shift left */
    c = b >> 1;         /* shift right */

    /* Use OpenRISC 1000 mnemonics for extended ALU ops (from header mnemonics):
       We'll force add with carry, subtract with extended flag,
       and rotate left using known valid mnemonics
    */

    asm volatile(
        /* add with carry: addc r3,r4,r5 encoded as "addc" in OpenRISC header */
        ".word 0xAC210000\n"  /* addc r3,r4,r5 — encoding from manual (placeholder) */

        /* subtract with extended flag / borrow: subfe r6,r7,r8 not recognized,
           use "subfe" encoded as .word with known op code or nearest variant */
        ".word 0xB0000000\n"  /* placeholder for subfe r6,r7,r8 */

        /* rotate left: use "rotl" encoding */
        ".word 0x92000000\n"  /* placeholder for rotl r9,r10,r11 */
        :
        :
        : "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11"
    );

    return 0;
}