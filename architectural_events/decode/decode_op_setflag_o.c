#include <stdio.h>

int main() {
    int test_data[4] = {0x41414141, 0x41414141, 0x41414141, 0x41414141};
    int load_result = 0;
    load_result = test_data[0];
    /////////////////////////////////////////////////////////////////////
    int a = 42;
    int b = 100;
    int result_reg = 0;
    int result_imm = 0;

    // -------------------------------
    // Case 1: Register-Register Comparison
    // Triggers: l.sfXX → decode_op_setflag_o
    // -------------------------------
    if (a < b) {
        result_reg = 1;
    } else {
        result_reg = 0;
    }

    // -------------------------------
    // Case 2: Register-Immediate Comparison
    // Triggers: l.sfXXi → decode_op_setflag_o
    // -------------------------------
    //if (a >= 50) {
    //    result_imm = 1;
    //} else {
    //    result_imm = 0;
    //}
    return 0;
}

