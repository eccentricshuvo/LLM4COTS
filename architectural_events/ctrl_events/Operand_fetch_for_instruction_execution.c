#include <stdint.h>

int main(void)
{
    uint32_t a = 5, b = 10, c;

    // Perform some arithmetic operations to trigger operand fetch from the register file
    c = a + b;               // Fetch operands a and b
    c = c * 2;               // Fetch operand c
    c = c - a;               // Fetch operands c and a

    return (int)c;          // Prevent optimization
}