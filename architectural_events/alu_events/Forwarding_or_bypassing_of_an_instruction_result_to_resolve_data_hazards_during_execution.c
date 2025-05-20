int main(void)
{
    /* Trigger forwarding by issuing dependent arithmetic instructions back to back.
     * add r3,r4,r5 followed immediately by sub r6,r3,r7 forces rfb_i to forward r3's result to sub.
     * This creates a data hazard resolved by operand forwarding/bypassing inside the pipeline.
     */
    volatile int r4 = 10;
    volatile int r5 = 20;
    volatile int r7 = 5;

    int r3 = r4 + r5;       /* add */
    int r6 = r3 - r7;       /* uses r3 immediately, triggering forwarding rfb_i */

    (void)r6; /* Avoid optimization */

    return 0;
}