/*
 * description: 1x3 matrix - multiply benchmarking
 *
 * |h11 h12 h13| |x1| |y1| | h11*x1+h12*x2+h13*x3 |
 * |h21 h22 h23| * |x2| = |y2| = | h21*x1+h22*x2+h23*x3 |
 * |h31 h32 h33| |x3| |y3| | h31*x1+h32*x2+h33*x3 |
 *
 * Element are to store in following order:
 *
 * matrix h[9]={h11,h12,h13, h21,h22,h23, h31,h32,h33}
 * vector x[3]={x1,x2,x3}
 * vector y[3]={y1,y1,y3}
 */
#include<stdio.h>
int main()
{
    int f,i=0;
    int h[9]={0}, x[3]={0}, y[3]={0};
    FILE *input = fopen("../input/3.txt","r");
    for(i = 0; i<9; i++) fscanf(input, "%d", &h[i]);
    for(i = 0; i<3; i++) fscanf(input, "%d", &x[i]);
    for(i = 0; i<3; i++) fscanf(input, "%d", &y[i]);
    fclose(input);
    int *p_x = &x[0] ;
    int *p_h = &h[0] ;
    int *p_y = &y[0] ;

    //  original c code
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
    for (i = 0 ; i < 3; i++)
    {

        p_x = &x[0] ;
        for (f = 0 ; f < 3; f++){
            asm volatile(
                "lw t1, 0(%[PH])\n\t" // load value at p_h into register t1
                "lw t2, 0(%[PX])\n\t" // load value at p_x into register t2
                "lw t3, 0(%[PY])\n\t" // load value at p_y into register t3
                "mul t0, t1, t2\n\t" // multiply t1 and t2 and store result in register t3
                "add t3, t3, t0\n\t" // calculate the memory address (ptr + offset)
                "sw t3, 0(%[PY])\n\t"    // store the value at the memory address
                "addi %[PH], %[PH], 4\n\t" // increment p_h
                "addi %[PX], %[PX], 4\n\t" // increment p_x
                :[I] "+r" (i), [PH] "+r" (p_h), [PX] "+r" (p_x), [PY] "+r" (p_y) // output operands
                : // no input operands
                : "t0","t1","t2","t3"// clobbered registers
            );
        }
        p_y++;
    }

    p_y = &y[0];
    for(i = 0; i<3; i++)
        printf("%d \n", *p_y++);
    return(0) ;

}