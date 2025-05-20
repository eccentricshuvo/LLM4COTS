#include <stdio.h>
void target_function_direct() {
    // Called directly → triggers l.jal
}

void target_function_indirect() {
    // Called via pointer → triggers l.jalr
}

int main() {
    // Case 1: Direct function call — triggers l.jal
    target_function_direct();

    // Case 2: Indirect function call — triggers l.jalr
    void (*func_ptr)() = target_function_indirect;
    func_ptr();

    return 0;
}

