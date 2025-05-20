#include <limits.h>

int main(void)
{
    int x = INT_MAX; // Set x to the maximum representable value for int
    x += 1;          // This operation will cause an overflow

    return x;        // Avoid optimization away
}