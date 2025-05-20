// C test program to trigger register file bypassing during instruction execution

int main() {
    // Using minimal inline assembly to trigger the event
    asm volatile(
        "l.add r3, r4, r5  /* id: l.add, encoding: 0010 110100 011 00011 00000 */" // Executing an add instruction to trigger register file bypassing
        "/* example_hex: 0x2DA60003 */"
        :::"r3", "r4", "r5"
    );

    return 0;
}