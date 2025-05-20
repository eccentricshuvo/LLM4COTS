#include <stdint.h>

volatile uint8_t mem[16] = {0};

/* Access memory with byte, halfword, and word sizes at different alignments */
/* to trigger dbus_bsel signal due to byte-level select lines assertion. */

int main(void)
{
    /* Store a byte at unaligned address */
    volatile uint8_t *p_byte = &mem[1];
    *p_byte = 0xAA;

    /* Store a halfword at 2-byte aligned address */
    volatile uint16_t *p_halfword = (uint16_t *)&mem[2];
    *p_halfword = 0xBBBB;

    /* Store a word at 4-byte aligned address */
    volatile uint32_t *p_word = (uint32_t *)&mem[4];
    *p_word = 0xCCCCCCCC;

    /* Load a byte from unaligned address */
    volatile uint8_t lb = mem[3];

    /* Load a halfword from 2-byte aligned address */
    volatile uint16_t lh = *(uint16_t *)&mem[6];

    /* Load a word from 4-byte aligned address */
    volatile uint32_t lw = *(uint32_t *)&mem[8];

    /* Prevent compiler optimizing reads/writes away */
    if (lb == 0xFF) return 1;
    if (lh == 0xFFFF) return 2;
    if (lw == 0xFFFFFFFF) return 3;

    return 0;
}