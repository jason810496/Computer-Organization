#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "lab2_q2.h"

int main(int argc, char *argv[]){
    input(atoi(argv[1]));

    short int *p_x = &x[0];
    short int *p_tmp1 = &tmp1[0];
    short int *p_tmp2 = &tmp2[0];

    int four_cnt = 0;
    int five_cnt = 0;

    /* original C code 
    for (int i = 0; i < 8; i++){
        if (flag == 1)
            break;

        for (int j = 0; j < 8; j++){
            if ((p_x[i] + p_x[j]) == target){
                flag = 1;
                break;
            }
        }
    }*/

    printf("===== Lab 2 question 2 =====\n");
    /* you should answer here */
    asm volatile(
        #include "lab2_q2_ans.c"

        : [p_x] "+r"(p_x), [p_tmp1] "+r"(p_tmp1), [p_tmp2] "+r"(p_tmp2), [target] "+r"(target), [flag] "+r"(flag), [CPI3_cnt] "+r"(CPI3_cnt), [CPI4_cnt] "+r"(CPI4_cnt), [CPI5_cnt] "+r"(CPI5_cnt)
    );

    if (flag == 1)
        printf("YES\n");
    else
        printf("NO\n");
    
    int cpu_time = (CPI3_cnt*3 + CPI4_cnt*4 + CPI5_cnt*5)*cycle_time;
    printf("CPU Time: %d\n", cpu_time);

    return 0;
}

void input(int number){
    if(number <= 0 || number > 10){
        printf("\nInvalid input!!!! (TEST_DATA please enter 1~10)\n");
        exit(0);
    }
    
	int fd;
	char *buf = (char *)calloc(3000, sizeof(char));
	
	fd = open("./lab2_q2_input.txt", O_RDONLY);
	if(fd < 0)
		exit(EXIT_FAILURE);
	if(read(fd, buf, 3000) < 0)
		exit(EXIT_FAILURE);

	char *token;
    token = strtok(buf, " ");
	int count = 0;
    
	for(int i = 0; i < number * 9; ++i){
		if(i >= (number - 1) * 9){
			if(count < 8)
				x[count] = atoi(token);
			else
				target = atoi(token);
			count++;
		}
		token = strtok(NULL, " ");	
    }
}