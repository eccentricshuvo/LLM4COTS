/* Test program to provoke dc_we (data cache write enable) signal on OpenRISC-1000.
 * Strategy:
 *  - Execute a sequence of store instructions to a cacheable memory region.
 *  - Ensure cache is enabled and no exceptions occur.
 *  - Use pure C with volatile to prevent optimization removal of stores.
 */

volatile int mem[8] = {0};

int main(void)
{
    for (int i = 0; i < 8; ++i)
    {
        mem[i] = i + 1;      /* Store instruction to cacheable memory */
    }

    /* Further stores to provoke repeated dc_we assertions */
    for (int i = 0; i < 8; ++i)
    {
        mem[i] = mem[i] * 2;
    }

    return 0;
}