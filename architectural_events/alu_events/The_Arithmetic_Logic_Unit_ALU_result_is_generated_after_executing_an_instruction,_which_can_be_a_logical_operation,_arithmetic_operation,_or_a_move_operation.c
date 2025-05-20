int main(void)
{
    /* Test logical operations: AND, OR, XOR */

    unsigned int a = 0xF0F0F0F0u;
    unsigned int b = 0x0FF00FF0u;

    unsigned int and_res = a & b;
    unsigned int or_res = a | b;
    unsigned int xor_res = a ^ b;

    /* Test arithmetic operations: addition, subtraction */

    int c = 123456;
    int d = 654321;

    int add_res = c + d;
    int sub_res = d - c;

    /* Test move operations: register to register (in C just assignment) */

    unsigned int move_res = or_res;

    /* Test load/store and branch instructions are not needed here - these do not directly affect alu_result_o */

    /* Use variables volatily to prevent optimisation away */
    volatile unsigned int v_and = and_res;
    volatile unsigned int v_or = or_res;
    volatile unsigned int v_xor = xor_res;
    volatile int v_add = add_res;
    volatile int v_sub = sub_res;
    volatile unsigned int v_move = move_res;

    /* Return combined result to prevent removal */
    return (int)(v_and ^ v_or ^ v_xor) + v_add - v_sub + (int)v_move;
}