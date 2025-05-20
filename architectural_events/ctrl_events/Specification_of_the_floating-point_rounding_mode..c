#include <stdint.h>

int main() {
    uint32_t rounding_mode = 0x3;  // Example value for rounding mode

    asm("l.mtspr r0, %0, 6" :: "r"(rounding_mode));

    return 0;
}