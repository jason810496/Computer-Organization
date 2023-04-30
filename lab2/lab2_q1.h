#ifndef _CONSTANT_H_
#define _CONSTANT_H_

static short int h[16] = {0};
static short int x[16] = {0};
static short int y[16] = {0};

// Given baseline counters in Question 1
int arith_cnt = 0;
int lw_cnt = 0;
int sw_cnt = 0;
int others_cnt = 0;

// Given baseline CPI
const int arith_CPI = 4;
const int lw_CPI = 5;
const int sw_CPI = 4;
const int others_CPI = 3;

// Cycle count
int baseline_cycle_count = 0;
int improved_cycle_count = 0;

// Given cycle time
const int cycle_time = 384; // us

// CPU time
float baseline_cpu_time = 0.0;
float improved_cpu_time = 0.0;

void init();
void baseline();
void improved();
void input(int);

#endif