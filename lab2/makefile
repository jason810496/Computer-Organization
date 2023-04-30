MAKEFLAGS += --no-print-directory

PK_PATH = /opt/riscv/riscv64-unknown-linux-gnu/bin/pk
TEST_DATA = 0

score:
	@./test

run_lab2_q1: lab2_q1
	@spike --isa=RV64GCV $(PK_PATH) lab2_q1 $(TEST_DATA)

run_lab2_q2: lab2_q2
	@spike --isa=RV64GCV $(PK_PATH) lab2_q2 $(TEST_DATA)

run_lab2_q1_insert: lab2_q1_insert
	@spike --isa=RV64GCV $(PK_PATH) lab2_q1_insert $(TEST_DATA)

run_lab2_q2_insert: lab2_q2_insert
	@spike --isa=RV64GCV $(PK_PATH) lab2_q2_insert $(TEST_DATA)

lab2_q1: lab2_q1.c
	@riscv64-unknown-linux-gnu-gcc -march=rv64gcv -static -o lab2_q1 lab2_q1.c

lab2_q2: lab2_q2.c
	@riscv64-unknown-linux-gnu-gcc -march=rv64gcv -static -o lab2_q2 lab2_q2.c

lab2_q1_insert: lab2_q1.c
	@riscv64-unknown-linux-gnu-gcc -march=rv64gcv -static -o lab2_q1_insert lab2_q1.c

lab2_q2_insert: lab2_q2.c
	@riscv64-unknown-linux-gnu-gcc -march=rv64gcv -static -o lab2_q2_insert lab2_q2.c

clean:
	@rm -f lab2_q1 lab2_q2 lab2_q1_insert lab2_q2_insert lab2_q1_baseline_insert.c lab2_q1_improved_insert.c lab2_q2_ans_insert.c