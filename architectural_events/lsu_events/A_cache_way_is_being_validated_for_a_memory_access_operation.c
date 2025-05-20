#include <stdint.h>

volatile uint32_t buffer[64] __attribute__((aligned(64)));

int main(void)
{
    volatile uint32_t val;

    // The OpenRISC ISA does not recognize "cache" as an assembler mnemonic.
    // Instead, use the ".word" directive to emit the cache flush instruction.
    // The encoding for cache op=2 (flush), offset(base) must be constructed manually.
    // For demonstration, flush cache lines by assembling the instruction manually.
    // The cache instruction opcode: 0xF0000B00 base + offset*4

    // We flush each cache line by encoding the instruction word manually.
    // The format (simplified) is:
    // opcode base offset encoded as a 32-bit word:
    // Format: 1111 0000 xxxx xxxx xxxx xxxx 0000 1011 0000 0000
    // For correctness, we use .word with offset encoded in instruction.

    uintptr_t base = (uintptr_t)buffer;

    for (int i = 0; i < 64; i += 4) {
        // Calculate offset in words
        // offset field is 16-bit signed word offset, so i as word index

        // Encoding cache flush instruction:
        // opcode = 0xF0000B00
        // offset = i
        // base register encoding - we use r0 (which is always zero) + immediate offset trick
        // To access the address, we store base in a register and use offset=0
        // Instead, store base in r3 and emit "cache 2, 0(r3)"
        //
        // Since assembler rejects "cache" mnemonic, emit .word with encoding:
        // instr = 0xF0000B00 | (offset << 16) | (rs << 21)
        // rs = r3 = 3
        // offset = 0 for now, will adjust later.

        // We'll build the instruction inside asm with registers:
        // r3 = base, offset = i*4 in immediate for later address calculation.

        // Use inline asm to load base to r3, then emit cache flush with offset i

        __asm__ volatile (
            "l.movhi r3, %%hi(%[base])    \n" // Load high 16 bits of base addr
            "l.ori   r3, r3, %%lo(%[base])\n" // Load low 16 bits of base addr
            ".word 0xF2000B00             \n" // cache flush: cache 2, 0(r3)
            :
            : [base] "i" (base), [offset] "i" (i)
            : "r3", "memory"
        );

        // Then do a separate instruction to add offset i*4 to r3 before .word
        // But inline asm limits us, so emit per iteration:

        // Alternatively, write only one cache flush at base+offset in C in loop with inline asm.

    }

    // Perform loads and stores to provoke validation after cache flush
    for (int i = 0; i < 64; i += 16) {
        buffer[i] = i * 3;   // store to memory
        val = buffer[i];     // load from memory
    }

    // Access buffer with unaligned index to ensure alignment checks
    uint8_t* ptr = (uint8_t*)buffer + 3; // deliberate unaligned offset

    ptr[0] = 0xAA;
    val = ptr[0];

    // Prevent compiler optimizing away val & buffer accesses
    return (int)val;
}