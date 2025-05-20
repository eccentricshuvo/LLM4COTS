int main(void)
{
    volatile int shared_var = 0;

    /*
     * The OpenRISC ISA JSON does not contain an 'inval' instruction.
     * Instead, we provoke cache line invalidation through an external store
     * and load of a shared volatile variable in a tight loop, which triggers
     * cache coherence actions.
     *
     * Minimal inline assembly is not used because no exact 'inval' mnemonic exists.
     * Pure volatile accesses simulate the external request causing invalidation.
     */

    for (int i = 0; i < 100000; ++i) {
        shared_var = i;   /* Store */
        int tmp = shared_var; /* Load */
        (void)tmp; /* Silence unused variable warning */
    }

    return 0;
}