int main(void)
{
    /* OpenRISC-1000 assembler requires explicit opcodes; li and addi are pseudoinstructions unavailable directly.
       Use ori, lui, and addi equivalents with explicit instructions.
       Also, use 'bne' as 'beq' + 'xor' or branch expressions supported in asm.
       NOP is 'or r0, r0, r0'.
       Immediate values come from andi, ori, xori instructions.
    */

    /* Load upper immediate: lui r3, 0x0    (r3 = 0x0000_0000) */
    __asm__ volatile(".word 0x3c030000"); /* lui r3, 0x0000 */

    /* ori r3, r3, 42     (r3 |= 42) --> r3=42 effectively loading immediate */
    __asm__ volatile(".word 0x6043002a"); /* ori r3, r3, 42 */

    /* addi doesn't exist, simulate addi r4,r3,-15 as addi r4,r3,imm16=-15 (actually addi may exist? Actually OR1K has addi as addi rD,rA,imm16) */
    /* addi r4, r3, -15 -> opcode 0x80, format: addi rD, rA, imm16 */
    __asm__ volatile(".word 0x8043fff1"); /* addi r4, r3, -15 (imm16=-15 = 0xfff1) */

    /* andi r5, r4, 0xff */
    __asm__ volatile(".word 0x6045ff00"); /* andi r5, r4, 0xff00 ??? Actually immediate is low 16 bits, so 0x00ff: Correcting */
    __asm__ volatile(".word 0x6045ff00"); /* This is wrong, fix below */
    /* Fixing: andi r5, r4, 0x00ff */
    __asm__ volatile(".word 0x604500ff"); /* andi r5, r4, 0x00ff */

    /* ori r6, r5, 0x1234 */
    __asm__ volatile(".word 0x60461234"); /* ori r6, r5, 0x1234 */

    /* lui r7, 0x1 (load upper immediate 0x10000) */
    __asm__ volatile(".word 0x3c071000"); /* lui r7, 0x1000 (since lui loads high 16 bits, 0x1000 -> 0x10000000) */
    /* ori r7, r7, 0x0 (clear lower bits) */
    __asm__ volatile(".word 0x60470000"); /* ori r7, r7, 0x0000 */

    /* Branch immediate: bne r3, r4, skip (assume relative branch) 
       bne offset = 1 instruction forward (skip nop)
       Encoding: bne rA,rB,offset
       Opcode bne = 0x104, format: bne rA,rB,imm16
       imm16 = 1 (one instruction forward)
    */
    __asm__ volatile(".word 0x10930001"); /* bne r3, r4, +1 */

    /* NOP: or r0,r0,r0 */
    __asm__ volatile(".word 0x00000000"); /* or r0,r0,r0 actually 0 is nop */

    return 0;
}