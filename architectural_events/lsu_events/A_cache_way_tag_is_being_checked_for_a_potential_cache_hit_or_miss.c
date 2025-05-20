#include <stdint.h>

volatile int array[1024] = {0};

int main(void)
{
    // Increase working set size further to exceed cache capacity and stress cache tag checks.

    for (int pass = 0; pass < 20; ++pass)
    {
        for (int i = 0; i < 1024; i += 8)
        {
            // Store operation to trigger cache tag check
            array[i] = pass + i;

            // Load operation to trigger cache tag check
            int tmp = array[i];
            (void)tmp;
        }
    }

    // Access array with different strides to increase cache set conflicts and force tag checks
    for (int stride = 1; stride <= 8; stride *= 2)
    {
        for (int i = 0; i < 1024; i += stride)
        {
            array[i] = array[i] + stride;
            int tmp = array[i];
            (void)tmp;
        }
    }

    return 0;
}