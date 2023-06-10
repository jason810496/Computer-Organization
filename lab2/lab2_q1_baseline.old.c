"li t4,16;"
"li t3,0;"
"addi %[lw_cnt], %[lw_cnt], 2;"
"outer:;"
    "addi t3, t3 , 1;" 
    "addi %[arith_cnt], %[arith_cnt], 1;"

    "lw t0, 0(%[h]);" 
    "lw t1, 0(%[x]);" 
    "addi %[lw_cnt], %[lw_cnt], 2;"

    "add t2, t0, t1;" 
    "addi %[arith_cnt], %[arith_cnt], 1;" 

    "sw t2,0(%[y]);"
    "addi %[sw_cnt], %[sw_cnt], 1;" 
    

    "addi %[h], %[h], 2;" 
    "addi %[x], %[x], 2;" 
    "addi %[y], %[y], 2;" 
    "addi %[arith_cnt], %[arith_cnt], 3;" 

"addi %[others_cnt], %[others_cnt], 1;" 
"bne t3, t4, outer;" 
