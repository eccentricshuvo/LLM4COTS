int main(void)
{
    volatile int a = 10;
    volatile int b = 20;
    volatile int c;

    /* 
     * Assuming the feature flag affects arithmetic instructions,
     * use a simple arithmetic instruction likely influenced by the flag.
     * Pure C arithmetic should trigger the pipeline execution result
     * depending on the feature flag in hardware.
     */

    c = a + b;  /* Addition operation */

    /* Use the result to avoid optimization */
    return c;
}