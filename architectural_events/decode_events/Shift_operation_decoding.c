#include <stdint.h>

void trigger_shift_decode() {
    uint32_t x = 1;
    x = x << 1;
    x = x >> 1;
    x = x << 2;
    x = x >> 2;
}

int main() {
    trigger_shift_decode();
    return 0;
}