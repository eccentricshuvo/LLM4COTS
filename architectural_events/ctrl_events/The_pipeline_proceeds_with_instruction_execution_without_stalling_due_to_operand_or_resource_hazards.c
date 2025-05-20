#include <stdint.h>

int main(void)
{
    uint32_t a = 5, b = 10, c; // Simple arithmetic to avoid stalls
    c = a + b; // No stalls, simple addition
    
    // Load and store operation to not cause stalls
    uint32_t array[2] = {1, 2};
    array[0] = c; // Store valid data
    uint32_t val = array[1]; // Load valid data

    // Avoid optimization
    return (int)val;
}