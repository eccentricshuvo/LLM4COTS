#include <limits.h>
int main(void)
{
    /* Signed overflow by adding 1 to INT_MAX */
    volatile int x = INT_MAX;
    volatile int y = x + 1;

    /* Use to prevent optimization away */
    return y;
}