#include <stdint.h>

volatile uint32_t mem[8] = { 0xA5A5A5A5u, 0x5A5A5A5Au, 0xFFFFFFFFu, 0x00000000u,
                             0x12345678u, 0xCAFEBABEu, 0xDEADBEEFu, 0x0u };

int main(void)
{
    for (int i = 0; i < 8; ++i)
        (void)mem[i];

    uint32_t acc = 0;

    for (int i = 0; i < 1024; ++i)
    {
        uint32_t t = mem[i & 7];
        t += acc;
        asm volatile(".word 0x00000015" :: "r" (t), "r" (i + 3)); // Trigger decode bubble insertion
        acc = t * (i + 3);
    }

    return (int)acc;
}