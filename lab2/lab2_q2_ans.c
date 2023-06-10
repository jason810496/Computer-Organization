"li t0, 8;"
"li t1, 1;"    
   
"vsetvli t3, t0, e16;"
"vmv.v.x v0, %[target];"

"addi t2, %[p_x], 0;"        
"vle16.v v1, (t2);"
"vadd.vv v2, v1, v1;"
"vmseq.vv v3, v2, v0;"
"vfirst.m t5, v3;" 
"bltz t5, loop;"

"addi %[flag], %[flag], 1;"
"jal x0,last;"

"loop:;"
    "beq t1, t0, last;"

    "addi %[p_x], %[p_x], 2;"
    "lh t4, 0(%[p_x]);"
    "vmv.v.x v5, t4;"
    "vadd.vv v2, v1, v5;"
    "vmseq.vv v3, v2, v0;"
    "vfirst.m t5, v3;"   
    "addi %[p_x], %[p_x], 2;"
    "addi t1, t1, 1;"
"bltz t5, loop;"     

"li %[flag], 1;"
  
"last:;"

