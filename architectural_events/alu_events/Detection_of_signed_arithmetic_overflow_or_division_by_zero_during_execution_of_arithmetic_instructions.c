#include <stdint.h>

/* 
 * This test triggers signed overflow and division-by-zero exceptions:
 * - Addition overflow: INT32_MAX + 1
 * - Multiplication overflow: INT32_MIN * -1 (which also overflows in 2's complement)
 * - Division by zero: 1 / 0
 *
 * These operations provoke the processor's overflow_set_o signal in the execute stage.
 */

int main(void)
{
    volatile int32_t a, b, c, d, e;

    /* Addition overflow */
    a = INT32_MAX;
    b = 1;
    c = a + b; /* signed overflow addition */

    /* Multiplication overflow */
    a = INT32_MIN;
    b = -1;
    d = a * b; /* signed overflow multiplication */

    /* Division by zero */
    a = 1;
    b = 0;
    e = a / b; /* division by zero */

    /* Use volatile reads to prevent optimization */
    volatile int32_t sink = c + d + e;

    /* Avoid compiler removing code */
    return (int)sink;
}