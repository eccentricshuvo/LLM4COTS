int main(void)
{
    volatile int x = 5;
    volatile int y = 10;
    volatile int z = 0;

    /* Conditional branch with negated condition is triggered when branch is not taken
     * Example: if (x == y) { ... } else { z = 1; }
     * Since x != y, branch condition is false, branch not taken, b_neg should be asserted.
     */
    if (x == y)
    {
        z = 42;
    }
    else
    {
        z = 1;
    }

    return z;
}