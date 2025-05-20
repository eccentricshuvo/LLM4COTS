int main(void)
{
    unsigned int a = 5u;
    unsigned int b = 3u;
    unsigned int c = 1u;

    /* Force unsigned multiplication instructions by explicit separate operations */
    for (unsigned int i = 0; i < 20; i++) {
        c = a * b;
    }

    /* Use volatile to prevent optimization */
    volatile unsigned int sink = c;

    return (int)sink;
}