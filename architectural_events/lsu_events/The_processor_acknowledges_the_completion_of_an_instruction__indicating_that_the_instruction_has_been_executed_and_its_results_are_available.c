#include <stdint.h>

/* Volatile ensures memory operations are not optimized away */
volatile uint32_t mem[4] = { 0xDEADBEEF, 0x12345678, 0x0, 0xFFFFFFFF };

int main(void)
{
    uint32_t a = 0x10;
    uint32_t b = 0x20;
    uint32_t c = 0;

    /* Sequence:
        - Load from mem (load instruction completion triggers cpu_ack_o)
        - Store to mem (store instruction completion triggers cpu_ack_o)
        - Arithmetic and logical operations complete -> cpu_ack_o asserted.
        - Control flow with loop to exercise pipeline
    */

    for (int i = 0; i < 4; i++) {
        uint32_t val = mem[i];   /* Load operation: cpu_ack_o asserted on completion */
        val += a;                /* Arithmetic operation: triggers cpu_ack_o */
        mem[i] = val;            /* Store operation: triggers cpu_ack_o */
        c ^= val;                /* Logical operation: triggers cpu_ack_o */
    }

    /* Control flow: for loop ensures multiple instruction completions */

    /* Avoid unused variable warnings */
    return (int)(a + b + c);
}