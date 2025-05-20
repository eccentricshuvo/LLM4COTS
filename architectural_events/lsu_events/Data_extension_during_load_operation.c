#include <stdint.h>

/* 
 * This test triggers data extension during load operations by performing
 * loads of different sizes from memory. Sign-extended loads (lb, lh) and zero-extended
 * loads (ldb, ldh) can be provoked by using volatile signed and unsigned types.
 */

/* Volatile memory buffer to ensure loads are not optimized away */
volatile uint8_t mem[8] = { 0x80u, 0xFFu, 0x01u, 0x7Fu, 0x00u, 0x55u, 0xAAu, 0x0Fu };

int main(void)
{
    volatile int8_t  signed_byte;
    volatile int16_t signed_halfword;
    volatile int32_t signed_word;
    volatile uint8_t zero_byte;
    volatile uint16_t zero_halfword;
    volatile uint32_t zero_word;

    /* Sign-extended loads */
    signed_byte     = *((volatile int8_t*) &mem[0]);  /* lb: load byte with sign extension */
    signed_halfword = *((volatile int16_t*)&mem[0]);  /* lh: load halfword with sign extension */
    signed_word     = *((volatile int32_t*)&mem[0]);  /* lw: load word (no extension needed) */

    /* Zero-extended loads using unsigned types */
    zero_byte       = *((volatile uint8_t*)&mem[1]);  /* ldb: load byte zero extended */
    zero_halfword   = *((volatile uint16_t*)&mem[1]); /* ldh: load halfword zero extended */
    zero_word       = *((volatile uint32_t*)&mem[1]); /* ldw: load word no extension needed */

    /* Use the variables to avoid optimizing away */
    return zero_byte + zero_halfword + zero_word + signed_byte + signed_halfword + signed_word;
}