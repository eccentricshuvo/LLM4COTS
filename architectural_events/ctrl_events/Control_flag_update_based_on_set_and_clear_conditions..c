#include <stdint.h>

volatile uint32_t ctrl_flag_o;
volatile uint32_t ctrl_flag_clear;
volatile uint32_t spr_sr;
volatile uint32_t ctrl_flag_set;

void trigger_event() {
    spr_sr = 0x200; // set supervisor bit
    ctrl_flag_set = 1;
    ctrl_flag_clear = 0;
    asm volatile("mfmsr %0" : "=r" (spr_sr));
    asm volatile("mtmsr %0" : : "r" (spr_sr));
    ctrl_flag_o = ctrl_flag_set && !(ctrl_flag_clear && !(spr_sr & 0x200));
    ctrl_flag_clear = 1;
    ctrl_flag_o = ctrl_flag_set && !(ctrl_flag_clear && !(spr_sr & 0x200));
}

int main() {
    trigger_event();
    return 0;
}