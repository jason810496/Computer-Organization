"li t4,16\n\t"
"li t3,0\n\t"
"addi %[lw_cnt], %[lw_cnt], 2\n\t" // %[lw_cnt] 

"outer:\n\t"
    "addi t3, t3 , 1\n\t" 
    "addi %[arith_cnt], %[arith_cnt], 1\n\t" // %[arith_cnt]

    "lw t0, 0(%[h])\n\t" 
    "lw t1, 0(%[x])\n\t" 
    "addi %[lw_cnt], %[lw_cnt], 2\n\t" // %[lw_cnt] 

    "add t2, t0, t1\n\t" 
    "addi %[arith_cnt], %[arith_cnt], 1\n\t" // %[arith_cnt]

    "sw t2,0(%[y])\n\t"
    "addi %[sw_cnt], %[sw_cnt], 1\n\t" // %[arith_cnt]
    

    "addi %[h], %[h], 2\n\t" 
    "addi %[x], %[x], 2\n\t" 
    "addi %[y], %[y], 2\n\t" 
    "addi %[arith_cnt], %[arith_cnt], 3\n\t" // %[arith_cnt]

"bne t3, t4, outer\n\t" // if i < 16, go to outer loop
"addi %[others_cnt], %[others_cnt], 1\n\t" // %[lw_cnt]