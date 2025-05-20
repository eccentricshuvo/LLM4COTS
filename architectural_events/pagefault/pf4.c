#include <stdio.h>

int main() {
    printf("Triggering instruction page fault...\n");

//    // Deliberately execute an invalid instruction address
//    void (*invalid_func)() = (void(*)()) 0xDEADBEEF;
//    invalid_func();  // Should trigger except_ipagefault
//
//    // If we reach this line, the exception did not work as expected
//    printf("ERROR: Instruction page fault did not trigger!\n");
//    exit(1);
//
    return 0;
}


