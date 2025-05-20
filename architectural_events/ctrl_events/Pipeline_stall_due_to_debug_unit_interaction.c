int main(void)
{
    /*
     * To effectively trigger the pipeline stall due to debug unit interaction (du_stall_o),
     * repeatedly execute a debug-related instruction to keep the debug unit engaged.
     * The "l.trap" instruction causes a trap and stalls the pipeline due to debug checks.
     * Issuing multiple traps in a sequence with small computation will help ensure stalls.
     */

    // Repeated trap instructions to engage debug unit and provoke pipeline stall
    asm volatile (
        "l.trap 0\n\t"     /* id: "l.trap 0" */
        "l.trap 1\n\t"     /* id: "l.trap 1" */
        "l.trap 2\n\t"     /* id: "l.trap 2" */
    );

    // Some dependent arithmetic to maintain pipeline activity after traps
    volatile int a = 3;
    volatile int b = 4;
    volatile int c = 0;

    c = a + b;
    c = c ^ a;
    c = c - b;

    return c;
}