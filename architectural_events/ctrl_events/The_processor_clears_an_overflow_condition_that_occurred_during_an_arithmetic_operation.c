#include <stdint.h>

int main(void)
{
    volatile uint32_t result;
    volatile uint32_t a = 0xFFFFFFFFu; // Maximum 32-bit unsigned value
    volatile uint32_t b = 1;            // Value to cause overflow

    // Perform addition that will overflow
    result = a + b;

    // Clear overflow if needed (this is just to demonstrate triggering)
    asm volatile("l.add r0, r0, r0"); //  Clears the overflow condition

    return (int)result;
}