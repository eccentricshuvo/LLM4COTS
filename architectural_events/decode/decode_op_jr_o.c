#include <stdio.h>
void target_function() {
    // This function will be called indirectly to trigger l.jalr
}

int main() {
    void (*func_ptr)() = target_function;

    // Indirect function call — triggers l.jalr
    func_ptr();

    return 0;
}

