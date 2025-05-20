#include <stdint.h>

void trigger_register_file_destination_address_generation() {
    __asm__ volatile (
        "l.j 1f\n\t"
        "l.nop\n\t"
        "1:\n\t"
        "l.jal 2f\n\t"
        "l.nop\n\t"
        "2:\n\t"
    );
}

int main() {
    trigger_register_file_destination_address_generation();
    return 0;
}