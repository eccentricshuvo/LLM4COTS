#include <stdint.h>

/* 
 * Test program to provoke check_way_valid(0) signal related to cache way validation.
 * Strategy:
 * (1) Perform a store followed by a load to the same memory location.
 * (2) Include a memory barrier to enforce order and visibility.
 * (3) These operations exercise cache state transitions and memory bus interactions,
 *     triggering the cache way validation logic.
 */

volatile uint32_t data = 0;

int main(void)
{
    data = 0xA5A5A5A5u;         // Store to memory (sw equivalent)
    __asm__ volatile ("l.nop"); // Use a minimal volatile asm nop to prevent optimization

    // Use the documented memory barrier mnemonic "l.sync" per OpenRISC ISA JSON
    __asm__ volatile ("l.sync");  // Memory barrier instruction

    uint32_t temp = data;       // Load from memory (lw equivalent)

    // Use temp to avoid optimization-away
    if (temp == 0xA5A5A5A5u) {
        __asm__ volatile ("l.nop");
    }

    return 0;
}