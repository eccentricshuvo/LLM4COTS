#include <stdint.h>

void trigger_ctrl_flag_set_i() {
    uint32_t a = 0x00000001;
    uint32_t b = 0x00000002;

    a + b;
    a - b;
    a * b;
    a / b;

    a & b;
    a | b;
    a ^ b;

    a == b;
    a != b;
    a < b;
    a > b;

    a = 0;
    b = 0;

    a == b;
    a != b;
    a < b;
    a > b;

    a = 0xFFFFFFFF;
    b = 0x00000001;

    a + b;
    a - b;
    a * b;
    a / b;

    __asm__ volatile("add %0, %0, %1" : "+r" (a) : "r" (1));
    __asm__ volatile("sub %0, %0, %1" : "+r" (a) : "r" (1));
    __asm__ volatile("mul %0, %0, %1" : "+r" (a) : "r" (2));
    __asm__ volatile("div %0, %0, %1" : "+r" (a) : "r" (2));

    __asm__ volatile("and %0, %0, %1" : "+r" (a) : "r" (1));
    __asm__ volatile("or %0, %0, %1" : "+r" (a) : "r" (1));
    __asm__ volatile("xor %0, %0, %1" : "+r" (a) : "r" (1));

    __asm__ volatile("cmp %0, %1" : : "r" (a), "r" (b));
    __asm__ volatile("cmp %0, %1" : : "r" (a), "r" (b));
}

int main() {
    trigger_ctrl_flag_set_i();
    return 0;
}