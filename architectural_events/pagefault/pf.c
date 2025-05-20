#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

// OpenRISC system call numbers
#define SYS_exit 1

// Exit function for simulation
void exit_simulation() {
    asm volatile (
        "l.ori r3, r0, 0\n"  // Exit code 0
        "l.sys 1\n"          // Syscall exit (SYS_exit)
    );
}

int main() {
    printf("Attempting to access invalid memory...\n");

    // Trigger a page fault
    volatile uint32_t *ptr = (uint32_t *)0xDEADBEEF;
    uint32_t value = *ptr;  // Causes page fault

    // This should never execute
    printf("Unexpected: No page fault occurred!\n");

    // Exit simulation
    exit_simulation();
    
    return 0;
}

