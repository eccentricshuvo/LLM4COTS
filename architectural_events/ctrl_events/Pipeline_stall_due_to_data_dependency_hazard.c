#include <stdint.h>

volatile uint32_t data[4] = {0x100, 0x200, 0x300, 0x400};

int main(void)
{
    volatile uint32_t val;
    volatile uint32_t *ptr = data;

    // Use OpenRISC-specific mnemonics from JSON spec for instructions (lwz, addi, sw)
    // which are: lwz = load word and zero, addi = add immediate, sw = store word
    // Replace unknown instructions with .word encoding fallback where necessary (if mnemonics differ)
    // But here we use mnemonics from header JSON exactly

    for (int i = 0; i < 1024; ++i)
    {
        __asm__ __volatile__ (
            "lwz %0, 0(%1)       # lwz rX,0(rY)\n\t"
            "addi %0, %0, 1     # addi rX,rX,1\n\t"
            "sw %0, 0(%1)       # sw rX,0(rY)\n\t"
            : "=r"(val)
            : "r"(ptr + (i & 3))
            : "memory"
        );
    }

    return (int)val;
}