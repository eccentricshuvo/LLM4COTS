#include <stdint.h>

volatile uint32_t data[4] = {0x11111111u, 0x22222222u, 0x33333333u, 0x44444444u};

int main(void) {
    volatile uint32_t v;

    // Perform load operations
    v = data[0];
    v = data[1];
    v = data[2];

    // Perform store operations to different addresses
    data[0] = 0xAAAAAAAAu;
    data[1] = 0xBBBBBBBBu;

    // Another load after the stores
    v = data[3];

    // Avoid optimization of v
    return (int)v;
}