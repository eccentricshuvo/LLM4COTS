int main(void)
{
    int a = 10;
    int b;

    /* Use instructions valid for OpenRISC-1000 immediate selection:
       ori r3, r0, 10       (or immediate 10)
       subi r4, r3, 5       (subtract immediate 5)
       andi r5, r4, 0xFF    (and immediate 0xFF)
       ori r6, r5, 0xF0     (or immediate 0xF0)
       l.ahi r7, 0x1234     (load high immediate 0x1234)
    */

    asm volatile(
        "ori   r3, r0, 10\n"   /* id=ori    */
        "subi  r4, r3, 5\n"    /* id=subi   */
        "andi  r5, r4, 0xFF\n" /* id=andi   */
        "ori   r6, r5, 0xF0\n" /* id=ori    */
        "l.ahi r7, 0x1234\n"   /* id=l.ahi  */
        : "=r"(b) : "r"(a) : "r3","r4","r5","r6","r7"
    );

    /* Use the result so compiler does not optimize away */
    return b;
}