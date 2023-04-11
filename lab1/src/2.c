// substract vector b from vector a and store the result in vector c
#include <stdio.h>
int main (){
    int a[10] = {0}, b[10]= {0}, c[10] = {0};
    int i, arr_size = 10;
    FILE *input = fopen("../input/2.txt","r");
    for(i = 0; i<arr_size; i++) fscanf(input, "%d", &a[i]);
    for(i = 0; i<arr_size; i++) fscanf(input, "%d", &b[i]);
    for(i = 0; i<arr_size; i++) fscanf(input, "%d", &c[i]);
    fclose(input);
    int *p_a = &a[0];
    int *p_b = &b[0];
    int *p_c = &c[0];

    /* Original C code segment
    for (int i = 0; i < arr_size; i++){
    *p_c++ = *p_a++ - *p_b++;
    }
    */

    for (int i = 0; i < arr_size; i++){
        asm volatile (
            "lw t0, 0(%[PA])\n\t" // load value at p_a into register t0
            "lw t1, 0(%[PB])\n\t" // load value at p_b into register t1
            "sub t2, t0, t1\n\t" // subtract t1 from t0 and store result in memory at p_c
            "sw t2,%[PCVAL]\n\t"
            "addi %[PA], %[PA], 4\n\t" // increment p_a
            "addi %[PB], %[PB], 4\n\t" // increment p_b
            "addi %[PC], %[PC], 4\n\t" // increment p_c
            : [PCVAL] "=m" (*p_c), [PA] "+r" (p_a), [PB] "+r" (p_b), [PC] "+r" (p_c)
            : // no input operands
            : "t0", "t1" , "t2"// clobbered registers
        );
    }

    p_c = &c[0];
    for (int i = 0; i < arr_size; i++)
    printf("%d ", *p_c++);
    return 0;
}