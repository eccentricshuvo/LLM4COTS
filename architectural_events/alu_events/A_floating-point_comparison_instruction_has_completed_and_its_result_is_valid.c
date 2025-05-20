int main(void)
{
    volatile float a = 1.5f;
    volatile float b = 2.5f;
    volatile int result = 0;

    // The previous inline asm caused "impossible constraint" because
    // the alloc registers constraints "f" (float reg) and "r" (int reg)
    // cannot overlap and mixing inputs and outputs that way is invalid.
    // Also, OpenRISC inline asm constraints are limited.
    //
    // Instead, use three volatile float local vars to hold
    // values and results, and simpler inline asm with .word for fcmps

    volatile float fa = a;
    volatile float fb = b;
    volatile int cmp_res = 0;

    __asm__ volatile (
        "fmov.s %1, r2\n\t"   // Move 'fa' to fp reg r2
        "fmov.s %2, r3\n\t"   // Move 'fb' to fp reg r3
        ".word 0x5a530003\n\t" // fcmps r2, r3    (opcode for fcmps r2, r3)
        "mfcr %0\n\t"          // Move condition register to cmp_res
        : "=r"(cmp_res)
        : "r" (fa), "r" (fb)
        : "r2", "r3"
    );

    // Use the condition register bits to determine result of comparison:
    // Assume bit 31 (CR[0] less than) is set if fa < fb
    // bit 30 (CR[1] greater than) if fa > fb
    // bit 29 (CR[2] equal) if fa == fb
    // We want result = (a > b) ? 1 : 0;

    // CR bits: 31 (lt), 30 (gt), 29 (eq) in condition register
    result = ( (cmp_res & 0x40000000) != 0 ) ? 1 : 0;

    return result;
}