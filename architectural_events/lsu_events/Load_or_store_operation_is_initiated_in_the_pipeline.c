int main(void)
{
    volatile int array[4] = {1, 2, 3, 4};
    volatile int value;

    // Perform load and store operations to trigger ctrl_op_lsu
    for (int i = 0; i < 4; ++i) {
        value = array[i];   // load from array
        array[i] = value;   // store back to array
    }

    return 0;
}