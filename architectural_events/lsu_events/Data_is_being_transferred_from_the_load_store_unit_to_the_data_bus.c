#include <stdint.h>

/* Volatile array to force memory access */
volatile uint32_t mem[4] = {0x12345678U, 0x9ABCDEF0U, 0x0FEDCBA9U, 0x87654321U};

int main(void)
{
    volatile uint32_t x;

    /* Trigger load from mem to register - causes load/store unit to transfer data to dbus */
    x = mem[0];   /* Load word (lw) */
    
    /* Trigger store from register to mem - causes load/store unit to transfer data to dbus */
    mem[1] = x;   /* Store word (sw) */

    return 0;
}