#include <stdint.h>

volatile uint32_t data = 0x12345678u;

int main(void)
{
    uint32_t val1, val2;

    // Load from data to bring address into cache
    __asm__ volatile (
        "lwz %0, 0(%1)\n\t"    // lwz val1, data (OpenRISC load word zero)
        : "=r" (val1)
        : "r" (&data)
    );

    // Store to the same address to cause cache way match
    val1 += 1;
    __asm__ volatile (
        "stw %0, 0(%1)\n\t"    // stw val1, data (OpenRISC store word)
        :
        : "r" (val1), "r" (&data)
    );

    // Load again from same address to hit the cache way match
    __asm__ volatile (
        "lwz %0, 0(%1)\n\t"    // lwz val2, data
        : "=r" (val2)
        : "r" (&data)
    );

    // Use the loaded values to prevent optimization
    return (int)(val1 + val2);
}