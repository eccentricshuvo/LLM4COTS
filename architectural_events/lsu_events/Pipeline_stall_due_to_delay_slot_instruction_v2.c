int main(void)
{
    /* Loop with a branch instruction that has a delay slot.
     * The delay slot instruction immediately follows the branch.
     * This setup should provoke pipeline stall due to delay slot handling.
     */

    volatile int counter = 5;
    volatile int val = 0;

    while (counter > 0)
    {
        val++;           // Instruction in delay slot after branch
        counter--;       // Branch depends on counter

        if (counter > 0)
            continue;    // Branch with delay slot using pure C constructs

        break;
    }

    // Prevent optimization removing the loop
    return val;
}
