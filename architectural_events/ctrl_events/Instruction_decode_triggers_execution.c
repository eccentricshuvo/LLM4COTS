#include <stdint.h>

int main(void)
{
    volatile uint32_t data1 = 0xAABBCCDD;
    volatile uint32_t data2 = 0x11223344;
    
    // Trigger instruction execution to influence decode_valid_i signal
    asm volatile("l.add r3, %0, %1" : : "r"(data1), "r"(data2)); // l.add instruction to trigger execution
    
    return 0;
}