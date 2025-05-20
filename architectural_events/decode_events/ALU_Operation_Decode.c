void trigger_alu_operation_decode() {
    unsigned int a = 0x10;
    unsigned int b = 0x10;
    unsigned int c;

    c = a + b;  // opc_alu = 4'h4
    c = a - b;  // opc_alu = 4'h3
    c = a | b;  // opc_alu = 4'h5
}

int main() {
    trigger_alu_operation_decode();
    return 0;
}