#include <stdint.h>

volatile uint8_t mem8[4] = {0xAA, 0xBB, 0xCC, 0xDD};
volatile uint16_t mem16[2] = {0x1234, 0x5678};
volatile uint32_t acc = 0;

int main(void)
{
    /* Load byte and zero extend */
    uint32_t val8_0 = mem8[0]; /* Should trigger zero-extension in load byte */
    uint32_t val8_1 = mem8[1];

    /* Load halfword and zero extend */
    uint32_t val16_0 = mem16[0];
    uint32_t val16_1 = mem16[1];

    /* Use loaded zero-extended values in arithmetic */
    acc = val8_0 + val8_1 + val16_0 + val16_1;

    /* Prevent optimization by writing back */
    mem8[0] = (uint8_t)(acc & 0xFF);
    mem16[0] = (uint16_t)((acc >> 8) & 0xFFFF);

    return (int)acc;
}