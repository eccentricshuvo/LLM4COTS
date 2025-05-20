#include <stdint.h>

void trigger_data_cache_hit_detection() {
    uint32_t value = 0x12345678; // Example value to be stored in cache

    __asm__ volatile (
        "l.sw %0, 0(r0)\n\t"       // Store the value in the data cache at address 0
        "l.lw r1, 0(r0)\n\t"       // Load the value from the data cache (should hit)
        "l.nop\n\t"
        : 
        : "r" (value)
    );
}

int main() {
    trigger_data_cache_hit_detection();
    return 0;
}