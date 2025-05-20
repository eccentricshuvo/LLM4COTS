#include <stdint.h>

void trigger_imm_zext(void) {
    uint32_t imm = 0x0000FFFF;
    uint32_t result = imm & 0xFFFF;
}

int main() {
    trigger_imm_zext();
    return 0;
}