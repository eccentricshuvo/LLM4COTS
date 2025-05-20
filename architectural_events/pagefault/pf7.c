#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096  // 4KB page
#define NUM_PAGES 100   // Number of pages to access
#define INVALID_ADDR ((volatile int *)0xDEADBEEF)  // Unmapped address for page fault

// Large static array to trigger TLB translations
volatile int memory_region[NUM_PAGES * PAGE_SIZE / sizeof(int)];

void access_memory(volatile int *addr) {
    printf("Accessing virtual address: %p\n", addr);
    *addr = 42;  // Write operation to force translation
}

int main() {
    printf("Starting Virtual Memory Test on mor1kx...\n");

    // Access different memory locations to force translations
    for (int i = 0; i < NUM_PAGES; i++) {
        volatile int *addr = &memory_region[i * PAGE_SIZE / sizeof(int)];
        access_memory(addr);
    }

    // Cause a Page Fault by accessing an unmapped address
    printf("Triggering page fault by accessing invalid address: %p\n", INVALID_ADDR);
    *INVALID_ADDR = 123;  // This should trigger a page fault

    printf("Test completed (you should see a page fault above this line).\n");

    return 0;
}

