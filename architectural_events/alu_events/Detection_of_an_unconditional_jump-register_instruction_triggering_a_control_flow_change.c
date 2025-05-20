int foo(void)
{
    return 42;
}

int main(void)
{
    int (*func_ptr)(void);

    func_ptr = foo;

    // Prevent tail call optimization to ensure jump-register instruction emitted
    asm volatile("" ::: "memory");
    int val = func_ptr();
    asm volatile("" ::: "memory");

    (void)val; // prevent unused variable warning

    return 0;
}