int main(void)
{
    /* The event 'ext_result' requires instructions that produce results external
       to standard ALU operations. On OpenRISC, multiply instructions like mulu, muls
       are not recognized as named, but multiply is done via l.mul instruction or
       multiply with long result.

       The assembler error indicates 'mulu' is unrecognized.

       The correct OpenRISC instruction is 'mull' (long multiply).

       We'll use 'mull' which produces a 64-bit product in two registers (rd, rd+1).
       We capture the lower 32 bits.

       Syntax: mull rd, rs1, rs2

       This instruction triggers external result production.

       We'll use registers mapped to variables a, b, and output c.
    */

    volatile unsigned int a = 7, b = 3, c;

    asm volatile("mull %0, %1, %2" : "=r"(c) : "r"(a), "r"(b));

    return (int)c;
}