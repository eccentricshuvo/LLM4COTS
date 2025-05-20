/* 
 * Test program to trigger pipeline stall on data dependency hazard 
 * due to incomplete data from a previous load operation (dbus_dat_i).
 *
 * Strategy:
 *  (1) Load a word from memory.
 *  (2) Immediately perform an add that depends on the loaded data.
 * This enforces a load-use hazard, causing the pipeline to stall until
 * the load data is ready.
 */

#include <stdint.h>

volatile uint32_t memory[4] = { 0x11223344u, 0x55667788u, 0x99AABBCCu, 0xDDEEFF00u };

int main(void)
{
    volatile uint32_t val;
    volatile uint32_t result;

    val = memory[0];      /* Load word from memory */
    result = val + 1;     /* Dependent arithmetic operation */

    /* Prevent optimization of val and result */
    return (int)(result);
}