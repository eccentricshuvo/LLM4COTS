#include <stdint.h>

int main() {
    uint32_t control_flag_clear = 1;
    uint32_t atomic_flag_clear = 1;
    
    uint32_t cleared_flags = control_flag_clear | atomic_flag_clear;
    
    return 0;
}