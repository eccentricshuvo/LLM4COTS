#include <stdint.h>

#define ARRAY_SIZE 1024
volatile uint32_t data[ARRAY_SIZE];

int main(void)
{
    /* Initialize array with some values */
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        data[i] = i * 3 + 7;
    }

    /* Access the array in a pattern to trigger multiple cache line refills:
       - The array is large enough to exceed typical cache block widths.
       - Sequential load and store operations provoke the next_refill_adr calculation.
    */
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        uint32_t temp = data[i];    /* Load operation triggers cache refill address calc */
        temp += (uint32_t)i;
        data[i] = temp;             /* Store operation triggers cache refill address calc */
    }

    return 0;
}