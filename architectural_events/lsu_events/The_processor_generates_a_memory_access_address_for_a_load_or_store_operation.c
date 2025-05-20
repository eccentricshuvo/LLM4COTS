int main(void)
{
    volatile int array[4] = {1, 2, 3, 4};
    volatile int value = 42;
    volatile int index = 2;
    volatile int result;

    /* 
     * This sequence triggers memory address generation in load and store operations.
     * - Load from array at offset 'index'
     * - Store 'value' at offset 'index + 1'
     * - Arithmetic to alter the index for addressing
     */
    result = array[index];        /* load with register indirect + immediate offset */
    array[index + 1] = value;    /* store with register indirect + immediate offset */
    index = index + 1;           /* arithmetic affects address computation */
    result = array[index];

    /* Prevent optimization */
    return result;
}