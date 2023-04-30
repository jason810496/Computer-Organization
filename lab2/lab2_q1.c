#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "lab2_q1.h"
#include "answer.h"

int main(int argc, char *argv[]){
    init();
	input(atoi(argv[1]));
	baseline();

    init();
	input(atoi(argv[1]));
	improved();
	return 0;
}

void baseline(){
	short int *p_h = &h[0];
	short int *p_x = &x[0];
	short int *p_y = &y[0];
	
	/* original C code
	for (int i = 0; i < arr_size; i++){
		p_y[i] = p_h[i] + p_x[i];
	}
	*/

	asm volatile(
        #include "lab2_q1_baseline.c"

    : [h] "+r"(p_h), [x] "+r"(p_x), [y] "+r"(p_y), [arith_cnt] "+r"(arith_cnt), [lw_cnt] "+r"(lw_cnt), [sw_cnt] "+r"(sw_cnt), [others_cnt] "+r"(others_cnt)
    );

	printf("===== Baseline Question 1 =====\n");
    printf("output: ");
    for (int i = 0; i < 16; i++){
		printf(" %d ", y[i]);
	}
        
	printf("\n");

	printf("\n===== Baseline Question 2 =====\n");
	printf("arithmetic counter used: %d\n", arith_cnt);
	printf("lw counter used: %d\n", lw_cnt);
	printf("sw counter used: %d\n", sw_cnt);
	printf("others counter used: %d\n", others_cnt);
	
	macro_baseline_cycle_count

	printf("The total cycle count in this program: %d\n", baseline_cycle_count);

	printf("\n===== Baseline Question 3 =====\n");

	macro_baseline_cpu_time

	printf("CPU time = %f us\n", baseline_cpu_time);
}

void improved(){
	short int *p_h = &h[0];
	short int *p_x = &x[0];
	short int *p_y = &y[0];

	asm volatile(
		#include "lab2_q1_improved.c"

	: [h] "+r"(p_h), [x] "+r"(p_x), [y] "+r"(p_y), [arith_cnt] "+r"(arith_cnt), [lw_cnt] "+r"(lw_cnt), [sw_cnt] "+r"(sw_cnt), [others_cnt] "+r"(others_cnt)
	);

    printf("\n===== Vector Question 1 =====\n");
	printf("output: ");
	for (int i = 0; i < 16; i++){
		printf(" %d ", y[i]);
	}
	printf("\n");

	printf("\n===== Vector Question 2 =====\n");
	printf("arithmetic counter used: %d\n", arith_cnt);
	printf("lw counter used: %d\n", lw_cnt);
	printf("sw counter used: %d\n", sw_cnt);
	printf("others counter used: %d\n", others_cnt);
	
	macro_improved_cycle_count

	printf("The total cycle count in this program: %d\n", improved_cycle_count);

	printf("\n===== Vector Question 3 =====\n");

	macro_improved_cpu_time

	printf("CPU time = %f us\n", improved_cpu_time);
	printf("\n===== Vector Question 4 =====\n");
	float speedup = 0.0;
	if (improved_cpu_time != 0)
		speedup = baseline_cpu_time/improved_cpu_time;
	printf("V extension ISA faster %f times than baseline ISA\n", speedup);
}

void init(){
    arith_cnt = 0;
    lw_cnt = 0;
    sw_cnt = 0;
    others_cnt = 0;
	for(int i = 0; i < 16 ;++i){
		h[i] = 0;
		x[i] = 0;
		y[i] = 0;
	}
}

void input(int number){
	if(number <= 0 || number > 10){
        printf("\nInvalid input!!!! (TEST_DATA please enter 1~10)\n");
        exit(0);
    }

	int fd;
	char *buf = (char *)calloc(3000, sizeof(char));
	
	fd = open("./lab2_q1_input.txt", O_RDONLY);
	if(fd < 0)
		exit(EXIT_FAILURE);
	if(read(fd, buf, 3000) < 0)
		exit(EXIT_FAILURE);

	char *token;
    token = strtok(buf, " ");
	int count = 0;
    
	for(int i = 0; i < number * 32; ++i){
		if(i >= (number - 1) * 32){
			if(count < 16)
				h[count] = atoi(token);
			else
				x[count - 16] = atoi(token);
			count++;
		}
		token = strtok(NULL, " ");	
    }
}