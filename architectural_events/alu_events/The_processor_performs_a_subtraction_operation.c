int main(void)
{
    int a = 15;
    int b = 27;
    int c;

    /* Subtraction to provoke adder_do_sub_i */
    c = b - a;

    /* Use c to prevent optimization */
    if (c == 0)
        return 1;
    else
        return 0;
}