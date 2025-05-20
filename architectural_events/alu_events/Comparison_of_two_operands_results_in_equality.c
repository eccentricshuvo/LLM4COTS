int main(void)
{
    volatile int a = 42;
    volatile int b = 42;

    // The comparison a == b triggers the a_eq_b signal in the execution stage:
    // When a and b are equal, a_eq_b should be set.
    if (a == b)
    {
        // Do nothing; just a branch target.
        a = 1;
    }
    else
    {
        a = 0;
    }

    return 0;
}