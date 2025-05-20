#include <stdint.h>

/* 
 * This program triggers data cache access selection (dc_bsel) by performing multiple load and store operations,
 * while explicitly modifying the destination register.
 * Correct OpenRISC-1000 instruction syntax for l.lwz and l.sw uses offset as immediate and reg as base.
 * 
 * Addressing modes:
 *   l.lwz rd, offset(ra)  -- load from memory at address ra + offset
 *   l.sw rs, offset(ra)   -- store rs to memory at ra + offset
 * 
 * We must ensure the immediate offset is compile-time constant and register used for base address.
 * So we split offset_calculation from asm and pass immediate offset as a constant.
 */

volatile uint32_t data[32] = {0x12345678, 0x9ABCDEF0, 0x0, 0xFFFFFFFF,
                              0x13579BDF, 0x2468ACE0, 0xFEDCBA98, 0x76543210,
                              0xAAAAAAAA, 0x55555555, 0xABCDEF01, 0x10FEDCBA,
                              0xCAFEBABE, 0xDEADBEEF, 0x0F0F0F0F, 0xF0F0F0F0,
                              0x11111111, 0x22222222, 0x33333333, 0x44444444,
                              0x55555555, 0x66666666, 0x77777777, 0x88888888,
                              0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC,
                              0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x00000000};

int main(void)
{
    volatile uint32_t sink = 0;
    register uint32_t val asm("r3");
    register uint32_t base_addr asm("r4") = (uint32_t)&data;

    for (int i = 0; i < 32; ++i) {
        /* Load word zero-extended from base_addr + i*4 */
        asm volatile (
            "l.lwz %0, %1(%2)\n\t"     /* id=0x15 */
            : "=r"(val)
            : "I"(i * 4), "r"(base_addr)
            : /* no clobber */
        );

        val += i;

        /* Store word val back to base_addr + i*4 */
        asm volatile (
            "l.sw %0, %1(%2)\n\t"      /* id=0x16 */
            :
            : "r"(val), "I"(i * 4), "r"(base_addr)
            : /* no clobber */
        );

        sink += val;
    }

    return (int)sink;
}