volatile int array[8] = {10, 20, 30, 40, 50, 60, 70, 80};
volatile int sink;

int main(void)
{
    /* Perform multiple load operations to trigger data cache load completions */
    for (int i = 0; i < 8; i++) {
        int val = array[i]; /* load from memory */
        sink = val + 1;     /* use loaded data */
    }

    return 0;
}