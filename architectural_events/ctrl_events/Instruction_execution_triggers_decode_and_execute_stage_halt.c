#include <stdint.h>

int main(void)
{
    // Trigger a trap instruction to halt the decode and execute stages
    asm volatile(".word 0x0000006f"); // l.trap instruction encoding

    return 0;
}