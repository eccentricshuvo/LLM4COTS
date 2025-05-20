int main(void)
{
    // The event du_restart_from_stall relates to a debug unit stall requiring a pipeline restart.
    // Since the assembler doesn't recognize 'l.break' as an instruction modifier,
    // use the encoding directly with .word directive for the 'l.break' instruction.
    // Encoding for l.break is 0x0000000e.

    asm volatile(".word 0x0000000e"); // id=0x19 encoding=0x0000000e description=Instruction used to trigger breakpoints.

    // Introduce dependent load-store pair to cause pipeline stall after debug stall
    volatile int dummy = 0;
    volatile int mem = 42;
    dummy = mem;      // Load
    mem = dummy + 1;  // Store

    return 0;
}