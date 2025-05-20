int main(void)
{
    volatile int a = 1234;
    volatile int b = 5678;
    volatile int c = 0;

    // Force multiple operand fetches by performing volatile pointer accesses to memory
    // Use an array to ensure loads and stores happen.
    volatile int arr[4] = {a, b, 0, 0};

    // Load values from memory (loads trigger operand fetch)
    c = arr[0];
    c = c + arr[1];
    arr[2] = c;      // store triggers operand fetch
    c = arr[2];
    c = c ^ arr[0];

    // Multiple operations to trigger operand fetch on execution
    arr[3] = c * arr[1];
    c = arr[3] - arr[0];
    c = c & arr[1];
    c = c | arr[2];

    // Use volatile pointer to force memory accesses strictly
    volatile int *p = arr;
    c = p[3];
    p[0] = c;

    return c;
}