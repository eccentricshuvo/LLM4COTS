int foo(int x)
{
    if (x > 0) {
        return x * 2;
    } else {
        return -x;
    }
}

int main(void)
{
    int a = 10;
    int b = foo(a);

    for (int i = 0; i < 5; i++) {
        b += i;
        if (b > 20) {
            b -= 5;
        }
    }

    return b;
}