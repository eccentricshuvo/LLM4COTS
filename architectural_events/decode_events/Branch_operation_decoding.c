#include <stdio.h>

void trigger_branch_decoding() {
    int a = 10;
    if (a > 5) {
        a = a + 1;
    } else {
        a = a - 1;
    }
    int b = 20;
    if (b < 15) {
        b = b + 2;
    } else {
        b = b - 2;
    }
    int c = 30;
    if (c > 25) {
        c = c + 3;
    } else {
        c = c - 3;
    }
    while (c > 0) {
        c = c - 1;
    }
    int d = 40;
    do {
        d = d - 4;
    } while (d > 35);
    for (int i = 0; i < 10; i++) {
        d = d + i;
    }
    switch (d) {
        case 1:
            d = d + 10;
            break;
        case 2:
            d = d + 20;
            break;
        default:
            d = d + 30;
    }
}

int main() {
    trigger_branch_decoding();
    return 0;
}