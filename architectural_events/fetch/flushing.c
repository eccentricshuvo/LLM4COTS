#include <stdio.h>

#include "or1k-support.h"
#include "or1k-sprs.h"

void illegal_instr_exception_test() {
    // Emit illegal instruction: will trigger exception → flush
    asm volatile (".word 0xFFFFFFFF");  // Invalid opcode, triggers exception
}

int main() {
    int test_data[4] = {0x41414141, 0x41414141, 0x41414141, 0x41414141};
    int load_result = 0;
    load_result = test_data[0];
    /////////////////////////////////////////////////////////////////////
    
    // Step 1: Enable supervisor mode (optional — often enabled by default)
    // Step 2: Call function that causes illegal instruction → triggers exception
    illegal_instr_exception_test();

    // After exception handler runs, it will do l.rfe → triggers flush again
    // So the fetch pipeline is flushed twice:
    // - Once on exception
    // - Again on rfe (return from exception)

    return 0;
}
