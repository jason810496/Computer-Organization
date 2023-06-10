MAKEFLAGS += --no-print-directory

#cache setting
CACHE_SET = ''
CACHE_WAY = ''
CACHE_BLOCKSIZE = ''

PK_PATH = /home/ubuntu/riscv/riscv64-unknown-elf/bin/pk
FILE_NAME = ''
SPIKE_PATH = ${HOME}/Downloads/riscv-isa-sim/

test:
	@python3 test.py test
	@make clean

score:
	@python3 test.py build
	@make clean

run: a.out
	@spike --dc=$(CACHE_SET):$(CACHE_WAY):$(CACHE_BLOCKSIZE) --isa=RV64GC $(PK_PATH) a.out

compile: $(FILE_NAME)
	@riscv64-unknown-elf-gcc -march=rv64gc -static -o ./a.out $(FILE_NAME)

build:
	cd $(SPIKE_PATH)/build && ../configure --prefix=/home/ubuntu/riscv && make && sudo make install

origin:
	@cp -f ORIG_cachesim.cc $(SPIKE_PATH)/riscv/cachesim.cc
	@cp -f ORIG_cachesim.h $(SPIKE_PATH)/riscv/cachesim.h
	@make build

fifo:
	@cp -f FIFO_cachesim.cc $(SPIKE_PATH)/riscv/cachesim.cc
	@cp -f FIFO_cachesim.h $(SPIKE_PATH)/riscv/cachesim.h
	@make build

lru:
	@cp -f LRU_cachesim.cc $(SPIKE_PATH)/riscv/cachesim.cc
	@cp -f LRU_cachesim.h $(SPIKE_PATH)/riscv/cachesim.h
	@make build

lfu:
	@cp -f LFU_cachesim.cc $(SPIKE_PATH)/riscv/cachesim.cc
	@cp -f LFU_cachesim.h $(SPIKE_PATH)/riscv/cachesim.h
	@make build

self:
	@cp -f SELF_cachesim.cc $(SPIKE_PATH)/riscv/cachesim.cc
	@cp -f SELF_cachesim.h $(SPIKE_PATH)/riscv/cachesim.h
	@make build

clean:
	@rm -f *.out *.gif