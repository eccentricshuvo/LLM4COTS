/*
 * The OpenRISC-1000 assembler expects the mtspr instruction with immediate SPR
 * numbers, not registers for SPR specifiers. The correct mangling is:
 *   mtspr SPRnum, reg
 * where SPRnum is an immediate (e.g., 0x11), reg is a general purpose register.
 *
 * So we must fix inline asm to move the address into a register and then 
 * do two "mtspr" instructions using the immediate SPR number directly.
 * 
 * Syntax:
 *   mtspr <spr_num>, <reg>
 *
 * We'll use GCC extended asm with outputs and inputs to manage registers.
 */

#include <stdint.h>

volatile uint32_t shared_mem = 0xA5A5A5A5u;

#define SPR_DCACHECMD  0x11
#define SPR_DCACHEADDR 0x12

static inline void cache_flush(void *addr)
{
    uintptr_t a = (uintptr_t)addr;
    register uintptr_t reg_addr asm("r3") = a;
    asm volatile (
        "l.mtspr %0, %1\n"      // mtspr SPR_DCACHEADDR, r3
        "l.mtspr %2, %3\n"      // mtspr SPR_DCACHECMD, r4 (value 2)
        :
        : "i"(SPR_DCACHEADDR), "r"(reg_addr),
          "i"(SPR_DCACHECMD), "r"(2)
        : "r3", "memory"
    );
}

static inline void cache_invalidate(void *addr)
{
    uintptr_t a = (uintptr_t)addr;
    register uintptr_t reg_addr asm("r3") = a;
    asm volatile (
        "l.mtspr %0, %1\n"      // mtspr SPR_DCACHEADDR, r3
        "l.mtspr %2, %3\n"      // mtspr SPR_DCACHECMD, r4 (value 1)
        :
        : "i"(SPR_DCACHEADDR), "r"(reg_addr),
          "i"(SPR_DCACHECMD), "r"(1)
        : "r3", "memory"
    );
}

int main(void)
{
    volatile uint32_t *ptr = &shared_mem;

    // Load from shared address - brings data into cache
    uint32_t val1 = *ptr;

    // Flush cache line using SPR cache control
    cache_flush((void *)ptr);

    // Store new data - triggers snoop operation internally
    *ptr = 0x5A5A5A5Au;

    // Invalidate cache line using SPR cache control
    cache_invalidate((void *)ptr);

    // Load again, expected to cause dc_snoop_hit event due to cache snoop
    uint32_t val2 = *ptr;

    // Use values to prevent optimization
    if (val1 == val2) {
        return 1;
    } else {
        return 0;
    }
}