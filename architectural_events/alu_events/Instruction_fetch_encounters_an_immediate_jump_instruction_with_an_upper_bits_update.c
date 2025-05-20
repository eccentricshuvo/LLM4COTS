int main(void)
{
    /* Pure C jump to a label far enough away to require upper bits update in the immediate jump address */
    
    /* Use a large offset label */
    /* Large spacing to ensure immediate jump needs upper bits update */
    goto far_away;

    /* Filler code to pad */
    for (volatile int i = 0; i < 1024; i++) {}

far_away:
    return 0;
}