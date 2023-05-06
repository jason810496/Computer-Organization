"li a0,16\n\t"
"loop:\n\t"
    "vsetvli t0, a1, e32, ta, ma\n\t"
        "sub a0,a0,t0\n\t" // Decrement number done
        "slli t0, t0, 2\n\t" // Multiply number done by 4 bytes

        "vle32.v v0,(%[h])\n\t" // Get first vector
        "add %[h], %[h], t0\n\t"

        "vle32.v v1,(%[x])\n\t" // Get second vector
        "add %[x], %[x], t0\n\t"

        "vadd.vv v2, v0, v1\n\t" // Sum vectors
        "vse32.v v2,(%[y])\n\t"

        "add %[y], %[y], t0\n\t" // Bump pointer

"bnez a0, loop\n\t" // Loop back