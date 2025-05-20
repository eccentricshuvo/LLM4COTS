volatile int shared = 0;

/*
 * To reliably trigger the "Cache line invalidation acknowledgement received" event,
 * this test program performs repeated write accesses to the shared variable,
 * forcing cache coherence protocol activity.
 *
 * We insert multiple store and load operations on the volatile shared variable,
 * separated by 'l.nop' instructions from the approved ISA,
 * to increase chances of triggering invalidate_ack by provoking cache line invalidations.
 */

int main(void)
{
    // Initial write to shared variable
    shared = 1;

    // Insert 'l.nop' instructions for ordering and latency
    __asm__ volatile (
        "l.nop\t\n"   /* 0000000b: l.nop (id=11) */
        "l.nop\t\n"   /* 0000000b: l.nop (id=11) */
        :
        :
        : "memory"
    );

    // Additional write to 'shared' to trigger coherence invalidation
    shared = 2;

    // More 'l.nop' instructions
    __asm__ volatile (
        "l.nop\t\n"   /* 0000000b: l.nop (id=11) */
        "l.nop\t\n"   /* 0000000b: l.nop (id=11) */
        :
        :
        : "memory"
    );

    // Read from shared variable, triggers snoop for cache line coherence
    int tmp = shared;

    (void)tmp;

    return 0;
}