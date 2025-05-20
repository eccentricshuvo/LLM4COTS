#include <stdint.h>

int main(void)
{
    /* Create a buffer aligned on at least 4 bytes */
    uint32_t buffer[2] = {0x12345678, 0x9abcdef0};

    /* Misaligned pointer: offset by 1 byte (not multiple of 4) */
    uint8_t *misaligned_ptr = (uint8_t *)buffer + 1;

    /* Perform misaligned 4-byte load and store via pointers */
    /* Since ISO C allows aliasing these unaligned accesses,
       this should trigger the align_err_short event on OR1K */

    /* Load 4 bytes from misaligned address */
    volatile uint32_t val_load = *(volatile uint32_t *)misaligned_ptr;

    /* Store 4 bytes to misaligned address */
    *(volatile uint32_t *)misaligned_ptr = val_load;

    /* Use val_load to prevent optimization */
    return (int)(val_load & 0xFF);
}