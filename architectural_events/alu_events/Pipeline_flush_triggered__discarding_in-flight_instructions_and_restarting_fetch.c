int main(void)
{
    /* Trigger a trap exception causing pipeline flush by executing an illegal instruction.
       Use actual illegal instruction opcode for OpenRISC (e.g., 0x0, which is reserved/illegal). */
    asm volatile(".word 0x00000000" ::: "memory");
    return 0;
}