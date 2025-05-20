int main(void)
{
    volatile float a = 1.5f;
    volatile float b = 2.5f;
    volatile float c;

    /* Execute multiple floating-point operations with inline assembly to ensure FPU instructions */
    __asm__ volatile(
        "fadds  %1, %2, %0\n"  /* c = a + b */   // Encoding comment placeholder
        "fsubs  %2, %1, %0\n"  /* c = b - a */   // Encoding comment placeholder
        "fmuls  %1, %2, %0\n"  /* c = a * b */   // Encoding comment placeholder
        "fdivs  %1, %2, %0\n"  /* c = a / b */   // Encoding comment placeholder
        : "=f" (c)
        : "f"(a), "f"(b)
    );

    /* Use the result to prevent optimization */
    if (c == (a / b))
        return 0;
    else
        return 1;
}