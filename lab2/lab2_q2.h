#ifndef _CONSTANT_H_
#define _CONSTANT_H_

static short int x[8] = {0};
static short int tmp1[8] = {0}; // fuifill with curr x[i]
static short int tmp2[8] = {0}; // fulfill with target
static short int target = 0;
static int flag = 0;

// Given counters in Question 2
int CPI3_cnt = 0;
int CPI4_cnt = 0;
int CPI5_cnt = 0;

// Given cycle time
// e means that the value is equal in two ISA
const int cycle_time = 384; // us

void input(int);

#endif