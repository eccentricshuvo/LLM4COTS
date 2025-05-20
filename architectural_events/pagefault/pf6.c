#include <stdint.h>

#define UART_TX  0x90000000  // Example UART TX address

// Function to write a character to UART (for printing)
void uart_putc(char c) {
    *(volatile uint32_t*)UART_TX = (uint32_t)c;
}

// Function to print a string
void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

int main() {
    // Print "Hello, World!"
    uart_puts("Hello, World!\n");

    // Simple arithmetic operation
    int a = 10, b = 20;
    int sum = a + b;

    // Print the result
    uart_puts("Sum = ");
    uart_putc('0' + (sum / 10));  // Convert first digit to char
    uart_putc('0' + (sum % 10));  // Convert second digit to char
    uart_putc('\n');

    //while (1);  // Infinite loop to keep running
    return 0;
}

