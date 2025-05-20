int main(void)
{
    int i = 0;
    int count = 10;

    /* Loop to provoke conditional branch instructions */
    while (i < count)
    {
        i++;
    }

    /* Use if-else to provoke conditional branches */
    if (i == count)
    {
        i = 0;
    }
    else
    {
        i = count;
    }

    /* Use a goto statement to provoke an unconditional jump */
    if (i == 0)
        goto label_jump;

    i = -1;  /* This line will be skipped by the goto */

label_jump:
    return i;
}