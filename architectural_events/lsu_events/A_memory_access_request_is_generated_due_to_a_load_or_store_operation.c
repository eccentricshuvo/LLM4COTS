#include <stdint.h>

volatile uint32_t data[4] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321};

int main(void)
{
    /* Load and store operations to provoke dbus_req_o signal */
    volatile uint32_t tmp;
    
    for (int i = 0; i < 4; i++)
    {
        tmp = data[i];           /* Load word */
        data[i] = tmp ^ 0xdeadbeef; /* Store word */
    }

    /* Additional load and store with varying sizes and alignments */
    volatile uint16_t *ptr16 = (uint16_t *)((uintptr_t)data + 2); // unaligned
    volatile uint8_t *ptr8 = (uint8_t *)((uintptr_t)data + 3);    // unaligned

    uint16_t val16 = *ptr16;      /* Load halfword, possibly unaligned */
    *ptr16 = val16 ^ 0x55aa;      /* Store halfword */

    uint8_t val8 = *ptr8;         /* Load byte */
    *ptr8 = val8 ^ 0xaa;          /* Store byte */

    /* Prevent optimisation away */
    if (val16 == 0 || val8 == 0)
        return 1;

    return 0;
}