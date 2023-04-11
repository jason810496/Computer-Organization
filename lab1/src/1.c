// description: a = a - b
#include <stdio.h>
int main (){
    int a, b;
    FILE *input = fopen("../input/1.txt","r");
    fscanf(input, "%d %d", &a, &b);
    fclose(input);
    /* Original C code segment */
    // a = a - b;
    asm volatile(
        "sub %[A], %[A], %[B]\n\t" //AssemblerTemplate
        :[A] "+r"(a) // output operands
        :[B] "r" (b) // input operands
        
    );
    printf("%d\n", a) ;
    return 0;
}