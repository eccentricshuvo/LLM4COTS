#include <stdint.h>

volatile uint32_t mem[8] = { 0xA5A5A5A5u, 0x5A5A5A5Au, 0xFFFFFFFFu, 0x00000000u,
                             0x12345678u, 0xCAFEBABEu, 0xDEADBEEFu, 0x0u };

int main(void)
{
    uint32_t acc = 0;

    for (int i = 0; i < 1024; ++i)            /* < 100 k iterations */
    {
        uint32_t t = mem[i & 7];              /* (1) load */
        acc += t;                             /* (2) immediate use */
        asm volatile("l.add r4, r5, r6");    /* (3) generate dependency */
        acc += (t * (i + 1));                 /* (4) use result */
    }

    return (int)acc;
}