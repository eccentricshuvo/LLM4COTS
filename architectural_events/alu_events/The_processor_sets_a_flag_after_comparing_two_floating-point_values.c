int main(void)
{
    volatile float a = 3.14f;
    volatile float b = 2.71f;
    volatile int flag = 0;

    /* Compare two floats to set the condition flag in the FPU */
    if (a > b) {
        flag = 1;
    } else {
        flag = 0;
    }

    /* Use flag in a conditional expression to prevent optimization */
    return flag;
}