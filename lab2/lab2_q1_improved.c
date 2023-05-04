"lw t0, 0(%[h])\n\t" // load value at p_a into register t0
"lw t1, 0(%[x])\n\t" // load value at p_b into register t1
"add t2, t0, t1\n\t" // subtract t1 from t0 and store result in memory at p_c
"sw t2,0(%[y])\n\t"
"addi %[h], %[h], 4\n\t" // increment p_a
"addi %[x], %[x], 4\n\t" // increment p_b
"addi %[y], %[y], 4\n\t" // increment p_c