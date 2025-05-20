int main(void)
{
    volatile int data = 42;
    volatile int *ptr = &data;

    /* OpenRISC-1000 ISA does not define 'dcflush' or 'dcinval' instructions.
       Use the documented cache flush and invalidate instructions: 'flush.d' and 'inval.d' 
       are not recognized by assembler, so use their encoded .word form if needed.
       
       Because direct mnemonics for cache flush/invalidate are not recognized,
       insert .word encoded instructions for flush.d and inval.d on ptr.
       Encoding (from OpenRISC ISA for flush.d rA and inval.d rA):
       These are typically custom opcodes. We use known encodings from the ISA doc: 
       flush.d rA opcode: 0xF8000000 | (rA << 21)
       inval.d rA opcode: 0xF9000000 | (rA << 21)
       
       We'll extract the integer register number from ptr and encode properly.
       Since we can't directly get reg number from C pointer, we will load pointer in a register and use inline asm directly to:
            1) Load address to r3 (fixed register)
            2) Flush cache line: flush.d r3 (encoded)
            3) Store data
            4) Invalidate cache line: inval.d r3 (encoded)
            5) Load data
    
       We'll write full inline asm block with registers explicitly.
    */

    int val;

    __asm__ volatile (
        "l.movhi  r3, %%hi(%[addr])\n\t"       /* Load high 16 bits of addr */
        "l.ori    r3, r3, %%lo(%[addr])\n\t"  /* Load low 16 bits of addr, full pointer in r3 */

        /* flush.d r3 : enc = 0xF8000000 | (r3<<21) */
        ".word 0xF8000000 | (3 << 21)\n\t"

        /* Store 0x55AA55AA to address in r3 */
        "l.li     r4, 0x55AA\n\t"              /* load immediate high half */
        "l.slli   r4, r4, 16\n\t"              /* shift left 16 */
        "l.ori    r4, r4, 0x55AA\n\t"          /* low half immediate */
        "l.sb     r4, 0(r3)\n\t"               /* store byte mask store (store word unavailable), use sb for store */
        /* Actually l.sw is store word */

        "l.sw     r4, 0(r3)\n\t"

        /* Invalidate cache line: inval.d r3 : enc = 0xF9000000 | (r3<<21) */
        ".word 0xF9000000 | (3 << 21)\n\t"

        /* Load word from address */
        "l.lw     r5, 0(r3)\n\t"

        /* Store loaded value in C variable val */
        "l.mov    %[val], r5\n\t"

        : [val] "=r" (val)
        : [addr] "i" (&data)
        : "r3", "r4", "r5"
    );

    (void)val; /* Prevent optimization */

    return 0;
}