#include <stdint.h>

volatile uint32_t array[10] = {0}; 

int main(void) {
    // Trigger cache miss during a load operation
    uint32_t load_val = array[0];

    // Trigger cache miss during a store operation
    array[1] = 10;

    return 0;
}