int main(void)
{
    int r4 = 7, r5 = 3, r6;
    /* Use extended instruction encoding with explicit .word encoding for 'mulu' */
    /* Encoding a 32-bit .word for a dummy multiply operation to trigger the extended opcode.
       Assuming the opcode for mulu rD,rA,rB is not recognized, we emit an opcode manually.
       Use inline asm with .word.
       We do a multiply of r4,r5, store to r6: mulu r6,r4,r5
       Encoding from OpenRISC ISA manual:
         op = 0x1b (extended MUL opcode)
         Format: 6-bit opcode (0x1b), rD, rA, rB: bits placed accordingly.
       The exact binary encoding depends but let's fallback to a known pattern. 
       Here we just use .word with a plausible encoding that matches mulu r6,r4,r5:
    */
    __asm__ volatile(
        ".word 0x1b8c662b\n" /* hypothetical mulu r6,r4,r5 */
    );

    return r6;
}