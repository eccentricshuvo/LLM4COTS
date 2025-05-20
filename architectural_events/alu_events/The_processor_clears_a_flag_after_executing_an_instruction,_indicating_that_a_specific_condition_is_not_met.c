int main(void)
{
    int a = 5;
    int b = 10;
    volatile int flag_set = 0;

    /* 
     * To trigger flag_clear_o, perform a comparison that fails
     * causing the flag to be cleared.
     * 
     * Using pure C comparison that results false should suffice,
     * as condition flags in OpenRISC are set/cleared accordingly.
     */

    if (a > b)
    {
        flag_set = 1; /* Would set flag if true */
    }
    else
    {
        flag_set = 0; /* Flag cleared due to condition fail */
    }

    /* Prevent optimization */
    return flag_set;
}