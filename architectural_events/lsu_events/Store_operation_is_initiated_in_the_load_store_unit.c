int main(void)
{
    volatile int mem = 0;
    volatile int val = 42;

    /* Pure C store operation triggers ctrl_op_lsu_store_i:
     * store 'val' to 'mem', aligned normal memory address
     */
    mem = val;

    return 0;
}