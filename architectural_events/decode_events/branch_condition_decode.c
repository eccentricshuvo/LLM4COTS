#include <stdint.h>

void trigger_branch_condition_decode() {
    int x = 10;
    int y = 20;
    if (x > y) {
        x = x + 1;
    } else {
        y = y - 1;
    }
}

int main() {
    trigger_branch_condition_decode();
    return 0;
}