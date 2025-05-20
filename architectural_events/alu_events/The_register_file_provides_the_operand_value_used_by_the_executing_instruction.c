int main(void)
{
    volatile int r1 = 1;
    volatile int r2 = 2;
    volatile int r3;

    // Sequence of dependent arithmetic operations to ensure operand reads cause register file access,
    // forwarding, and hazard resolution in the pipeline:
    r3 = r1 + r2;  // r3 depends on r1 and r2
    r3 = r3 + 5;   // uses previous result r3 as operand
    r3 = r3 * 2;   // further dependency
    r3 = r3 - r1;  // uses r1 again with dependency on r3
    
    // Prevent optimizing away
    return r3;
}