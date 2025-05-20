#include <stdint.h>

/* Volatile array to force real memory accesses */
volatile uint32_t mem[16] = {0};

/* Use stores and loads to provoke address generation on dbus_adr */
int main(void) {
    /* Initialize addresses and values */
    volatile uint32_t *p = mem;

    /* Write different values to different memory locations */
    for (int i = 0; i < 16; i++) {
        p[i] = (uint32_t)(i * 3 + 7);
    }

    /* Read back with arithmetic on the pointer to vary addresses */
    uint32_t acc = 0;
    for (int i = 0; i < 16; i++) {
        acc += p[i];
    }

    /* Additional store/load to provoke generation through pointer arithmetic */
    for (int i = 15; i >= 0; i--) {
        p[i] = acc - (uint32_t)i;
        acc += p[i];
    }

    return (int)acc;
}