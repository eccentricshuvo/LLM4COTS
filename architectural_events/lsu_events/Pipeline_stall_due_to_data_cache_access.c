#include <stdint.h>

volatile uint32_t data[1024 * 16];  // Large array to exceed typical cache line capacity

int main(void)
{
    /* Initialize array with distinct values to avoid zero optimizations */
    for (uint32_t i = 0; i < 1024 * 16; i++) {
        data[i] = i * 3 + 7;
    }

    volatile uint32_t sink = 0;
    /* Access data with a large stride to cause non-sequential accesses,
       increasing likelihood of data cache stalls/refills and pipeline stalls. */
    for (uint32_t i = 0; i < 1024 * 16; i += 64) {
        sink += data[i];
    }

    /* Perform some stores interleaved with loads to further stress data cache */
    for (uint32_t i = 0; i < 1024 * 16; i += 128) {
        data[i] = (uint32_t)(sink ^ i);
        sink += data[i];
    }

    /* Use explicit word load/store OpenRISC instructions "lwz" and "stw" as per ISA mnemonics from header */
    asm volatile (
        "lwz r3, 0(%0)\n\t"   /* Load word from data[0] */
        "stw r3, 4(%0)\n\t"   /* Store word to data[1] */
        :
        : "r"(data)
        : "r3", "memory"
    );

    return (int)sink;
}