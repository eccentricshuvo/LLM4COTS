int main(void)
{
    volatile int data[4] = {0x11223344, 0x55667788, 0x99AABBCC, 0xDDEEFF00};
    volatile int sum = 0;

    /* Load data from the array to trigger data loading into the CPU */
    for (int i = 0; i < 4; i++)
    {
        sum += data[i];
    }

    /* Prevent optimization */
    return sum;
}