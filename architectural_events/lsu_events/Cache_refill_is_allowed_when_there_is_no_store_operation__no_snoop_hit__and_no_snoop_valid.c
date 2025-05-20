#include <stdint.h>

volatile uint32_t array[16] = {0};

int main(void)
{
    uint32_t sum = 0;

    // Perform repeated load operations to provoke cache refill allowance.
    // Ensure no store instructions executed to keep ctrl_op_lsu_store_i low.
    // Access a small, contiguous memory block to minimize snoop hits and snoop valid signals.
    // Use volatile array to prevent compiler optimization on loads.

    for(int i = 0; i < 1024; ++i) {
        for(int j = 0; j < 16; ++j) {
            uint32_t val;
            __asm__ volatile (
                "l.lw %0, 0(%1)\n"  // Correct OpenRISC load word instruction mnemonic
                : "=r" (val)
                : "r" (&array[j])
                : 
            );
            sum += val;
        }
    }

    // Use sum to prevent optimization away of the loop
    return (int)sum;
}