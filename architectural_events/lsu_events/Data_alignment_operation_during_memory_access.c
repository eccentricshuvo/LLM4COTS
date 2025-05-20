#include <stdint.h>

volatile uint8_t buffer[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

int main(void)
{
    volatile uint32_t val;

    /* Perform misaligned loads of different sizes to trigger dbus_dat_aligned
       - Load a byte from an unaligned address (buffer + 1)
       - Load a halfword from an unaligned address (buffer + 2)
       - Load a word from an unaligned address (buffer + 1)
    */

    val = *(volatile uint8_t *)(buffer + 1);
    val += *(volatile uint16_t *)(buffer + 2);
    val += *(volatile uint32_t *)(buffer + 1);

    /* Perform misaligned stores of different sizes
       - Store a byte to an unaligned address (buffer + 3)
       - Store a halfword to an unaligned address (buffer + 5)
       - Store a word to an unaligned address (buffer + 3)
    */

    *(volatile uint8_t *)(buffer + 3) = (uint8_t)(val & 0xFF);
    *(volatile uint16_t *)(buffer + 5) = (uint16_t)((val >> 8) & 0xFFFF);
    *(volatile uint32_t *)(buffer + 3) = val;

    return 0;
}