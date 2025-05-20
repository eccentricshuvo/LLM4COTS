int main(void)
{
    volatile int array[4] = {0, 1, 2, 3};
    volatile int value;

    // Trigger load instructions by accessing elements of the array
    value = array[0];
    value = array[1];
    value = array[2];
    value = array[3];

    // Prevent optimization of value
    return value;
}