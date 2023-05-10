"vsetvli t0, x0, e16 ,m2 ;"
"addi %[others_cnt], %[others_cnt], 1;"

"vle16.v v0, (%[h]) ;"
"vle16.v v4, (%[x]) ;"
"addi %[lw_cnt], %[lw_cnt], 2;"

"vadd.vv v8, v0, v4 ;"
"addi %[arith_cnt], %[arith_cnt], 1 ;"

"vse16.v v8, (%[y]) ;"
"addi %[sw_cnt], %[sw_cnt], 1;"