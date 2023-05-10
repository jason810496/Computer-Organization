"li a1,16\n\t"
"loop:\n\t"
    "vsetvli t0, a1, e16\n\t"
        "vle16.v v0,(%[h])\n\t" 
        "sub a1,a1,t0\n\t"
        "slli t0, t0, 4\n\t"
        "add %[h], %[h], t0\n\t"

        "vle16.v v1,(%[x])\n\t" 
        "add %[x], %[x], t0\n\t"

        "vadd.vv v2, v0, v1\n\t" 
        "vse16.v v2,(%[y])\n\t"

        "add %[y], %[y], t0\n\t" 

"bnez a1, loop\n\t" 