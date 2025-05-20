#include <stdint.h>

int main() {
    uint32_t ctrl_flag_set_i = 1;
    uint32_t atomic_flag_set_i = 0;  // Assuming atomic_flag_set_i is not asserted
    
    uint32_t ctrl_flag_set = ctrl_flag_set_i | atomic_flag_set_i;
    
    return 0;
}