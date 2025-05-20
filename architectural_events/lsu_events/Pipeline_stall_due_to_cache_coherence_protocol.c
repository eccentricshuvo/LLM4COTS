/*
 * Test program to provoke pipeline stall due to cache coherence protocol.
 *
 * Strategy:
 * - Use shared volatile variable to enforce memory accesses visible to potential coherence.
 * - Perform repeated writes and reads to the same variable, simulating contention.
 * - Add minimal inline assembly "sync" using exact `.word` encoding because "sync" mnemonic unrecognized.
 *   The sync instruction encoding for OpenRISC is 0x0000000b (example placeholder),
 *   use actual encoding from ISA doc if different.
 *
 * Note:
 * - This program must be run in a multi-core or multi-threaded simulator or environment
 *   that implements cache coherence protocol for this event to trigger.
 */

volatile int shared_var = 0;

int main(void)
{
    for (int i = 0; i < 100000; i++)
    {
        // Write to shared variable
        shared_var = i;

        // Insert sync instruction encoded as .word to enforce memory ordering / provoke pipeline stall
        asm volatile (".word 0x0000000b" ::: "memory"); // sync instruction encoding

        // Read from shared variable
        int val = shared_var;

        // Volatile prevents compiler optimizing out
        (void)val;
    }

    return 0;
}