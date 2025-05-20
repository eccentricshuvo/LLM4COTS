#include <stdio.h>

int main() {
    int a = 0x12345678;
    int b = 0x0F0F0F0F;
    int result_add, result_sub, result_and, result_or, result_xor;
    int result_ori, result_andi, result_xori;

    // ----------------------------------
    // Case 1: Register-based ALU ops (opc_insn == OR1K_OPCODE_ALU)
    // ----------------------------------
    result_add = a + b;       // ADD
    result_sub = a - b;       // SUB
    result_and = a & b;       // AND
    result_or  = a | b;       // OR

    // ----------------------------------
    // Case 2: Immediate-based ALU ops (ori, andi, xori)
    // These will be compiled as ORI, ANDI, XORI
    // ----------------------------------
    result_ori  = a | 0x00FF00FF;   // OR Immediate
    result_andi = a & 0xFF00FF00;   // AND Immediate
    result_xori = a ^ 0x0000FFFF;   // XOR Immediate
    return 0;
}

