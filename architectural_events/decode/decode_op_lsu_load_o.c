#include <stdio.h>

int main() {
    int test_data[4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
    int load_result = 0;

    // Regular Load Instruction: Triggers decode_op_lsu_load_o
    load_result = test_data[0];
    load_result = test_data[1];
    load_result = test_data[2];
    load_result = test_data[3];
    load_result = test_data[0];
    load_result = test_data[1];
    load_result = test_data[2];
    load_result = test_data[3];
    load_result = test_data[0];
    load_result = test_data[1];
    load_result = test_data[2];
    load_result = test_data[3];
    load_result = test_data[0];
    load_result = test_data[1];
    load_result = test_data[2];
    load_result = test_data[3];

    // Ensure result is used so it's not optimized out
    //printf("Loaded value: 0x%X\n", load_result);

    return 0;
}
