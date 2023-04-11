// description: matrix muliply without for loop
#include<stdio.h>
int main()
{
    int f,i=0;
    int h[9]={0}, x[3]={0}, y[3]={0};
    FILE *input = fopen("../input/4.txt","r");
    for(i = 0; i<9; i++) fscanf(input, "%d", &h[i]);
    for(i = 0; i<3; i++) fscanf(input, "%d", &x[i]);
    for(i = 0; i<3; i++) fscanf(input, "%d", &y[i]);
    fclose(input);
    int *p_x = &x[0] ;
    int *p_h = &h[0] ;
    int *p_y = &y[0] ;

    // original c code
    // /* Level 1 for loop */
    // for (i = 0 ; i < 3; i++)
    // {
    //     /* p_x points to the beginning of the input vector */
    //     p_x = &x[0] ;

    //     /* Level 2 for loop */
    //     /* do matrix multiply */
    //     for (f = 0 ; f < 3; f++)
    //     {
    //         *p_y += *p_h++ * *p_x++ ;
    //     }

    //     /* next element */
    //     p_y++ ;
    // }

    // inline asm code
    asm volatile(
        "li t6,3\n\t" // for loop limit
        "li t4,0\n\t"
        "outer:\n\t"
        "addi t4, t4 , 1\n\t" // increment i

            "li t5, 0\n\t" // reset inner loop counter to 0 for each outer loop iteration
            "mv %[PX] , %[X]\n\t"

            "inner:\n\t"
            "addi t5, t5 , 1\n\t" // increment j

                "lw t1, 0(%[PH])\n\t" // load value at p_h into register t1
                "lw t2, 0(%[PX])\n\t" // load value at p_x into register t2
                "lw t3, 0(%[PY])\n\t" // load value at p_y into register t3
                "mul t0, t1, t2\n\t" // multiply t1 and t2 and store result in register t3
                "add t3, t3, t0\n\t" // add t3 and t0 and store result in register t3
                "sw t3, 0(%[PY])\n\t"    // store the value at the memory address
                "addi %[PH], %[PH], 4\n\t" // increment p_h
                "addi %[PX], %[PX], 4\n\t" // increment p_x

            "bne t5, t6, inner\n\t" // if j < 3, go to inner loop
            "addi %[PY], %[PY], 4\n\t" // increment p_y

        "bne t4, t6, outer\n\t" // if i < 3, go to outer loop

        :[PH] "+r" (p_h), [PX] "+r" (p_x), [PY] "+r" (p_y) // output operands
        :[X] "r" (x) // input operands
        : "t0","t1","t2","t3","t4","t5","t6"// clobbered registers
    );

    p_y = &y[0];
    for(i = 0; i<3; i++)
        printf("%d \n", *p_y++);
    return(0) ;

}