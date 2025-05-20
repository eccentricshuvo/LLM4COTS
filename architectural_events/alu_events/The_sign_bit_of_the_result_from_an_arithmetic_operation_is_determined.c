int main(void)
{
    volatile int a = 10;
    volatile int b = -20;
    volatile int c;
    
    /* Perform addition, result should have sign bit set (negative) */
    c = a + b;
    
    /* Perform subtraction to get negative result */
    c = b - a;
    
    /* Perform multiplication with negative result */
    c = a * b;
    
    /* Perform division that results in negative value */
    c = b / a;
    
    /* Conditional based on sign bit of c */
    if (c < 0) {
        c = -c;
    }
    
    return (int)c;
}