int main(void)
{
    volatile int a = -3;
    volatile int b = 2;
    volatile int result = 0;

    if (a < b) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}