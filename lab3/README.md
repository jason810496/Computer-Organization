# Developing the L1 Cache for the μRISC-V Processor

## Reference to source code 
All the source code are list in `/home/ubuntu/riscv/include/riscv`.
```
ls /home/ubuntu/riscv/include/riscv
```
```
abstract_device.h                common.h             decode.h          isa_parser.h   mmu.h        simif.h
abstract_interrupt_controller.h  csrs.h               devices.h         log_file.h     platform.h   trap.h
cachesim.h                       debug_module.h       encoding.h        memtracer.h    processor.h  triggers.h
cfg.h                            debug_rom_defines.h  entropy_source.h  mmio_plugin.h  sim.h        vector_unit.h
```
The `cachesim.h` , `memtracer.h` and `common.h` are the header files we need to read.
### `cachesim.h`
Same as `ORIG_chchesim.h` in homework .
### `memtracer.h`
- `memtracer_t` class : 
    - `virtual bool interested_in_range(uint64_t begin, uint64_t end, access_type type) = 0;`
    - `virtual void trace(uint64_t addr, size_t bytes, access_type type) = 0;`
    - `virtual void clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval) = 0;`
- `memtracer_list_t` class : 
    - inhert from `memtracer_t` class 
    - private member : 
        - `std::vector<memtracer_t*> list;`
        - a vector store `memtracer_t` pointer call `list`
    - public members : 
        - `bool interested_in_range(uint64_t begin, uint64_t end, access_type type)`
            - loop through `list` and call `interested_in_range` method
        - `void trace(uint64_t addr, size_t bytes, access_type type)`
            - loop through `list` and call `trace`
        - `void clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval)`
            - loop through `list` and call `clean_invalidate`
        - `void hook(memtracer_t* h)`
            - add `h` to `list`
### `common.h`
Just a marco header file define some compile tags. ( Not important in this lab )

## Observation 

After tracing down the cachesim source code , I found some most critical part of this homework :
- in `cache_sim_t` class : 
    - `miss_handler`
    - `clean_invalidate`
- in `fa_cache_sim_t` class :
    - `tags` : ( the original is `std::map` data structure )
    - `check_tag` 
    - `victimize`

For implemetation , we just need to change `tags` member ( or add other helper members ) in `fa_cache_sim_t` with appropriate data structure depends on different cache policy .

## Implementation 
An experienced classmate , [Chumy](https://github.com/Jimmy01240397)  wrote a brute force script to enumerate all possible `Set` ,`Way` , and `BlockSize` .

Based on [original script](https://github.com/jason810496/Computer-Architecture/blob/main/lab3/chumy.sh) ,  I add some code to print best miss rate result in the end of `output.txt`. ( [bruteforce.sh](https://github.com/jason810496/Computer-Architecture/blob/main/lab3/bruteforce.sh) )

### ORIG
The original cachesim use `LFSR` ( Linear feedback shift register ) , 

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 50.0967 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 50.1563 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 50.3273 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 39.0397 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 39.0293 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 35.9197 %
=======================================================================
Data Cache Setting with: 2:1:32
Miss Rate: 35.4987 %
=======================================================================
Data Cache Setting with: 1:1:64
Miss Rate: 36.4023 %
=======================================================================
Policy: orig
The best data cache setting is:
Setting: Set = 2, Way = 1, BlockSize = 32
Miss Rate: 35.4987
```

### FIFO

For `FIFO` ( first in first out ) I use `std::list` with `std::pair` as element where `.first` is tag and `.second` is data block .
- update `tags` member :
```cpp
class fa_cache_sim_t : public cache_sim_t
{
 public:
  fa_cache_sim_t(size_t ways, size_t linesz, const char* name);
  uint64_t* check_tag(uint64_t addr);
  uint64_t victimize(uint64_t addr);
 private:
  static bool cmp(uint64_t a, uint64_t b);
  // std::map<uint64_t, uint64_t> tags;
  std::list<std::pair<uint64_t, uint64_t>> tags; // updated !!!
};
```
- update `check_tag` method : 
    - using `std::find_if` ( should include `algorithm` ) to find tag
- update `victimize` method : 
    - using `push_back` and `pop_back` as a FIFO queue

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 50.0967 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 50.1563 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 50.3273 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 39.0397 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 39.0293 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 35.9197 %
=======================================================================
Data Cache Setting with: 2:1:32
Miss Rate: 35.4987 %
=======================================================================
Data Cache Setting with: 1:1:64
Miss Rate: 36.4023 %
=======================================================================
Policy: fifo
The best data cache setting is:
Setting: Set = 2, Way = 1, BlockSize = 32
Miss Rate: 35.4987
```

### LFU

For `LFU` ( Least Frequently Used ) I add another member `freq` ( which type is `std::map` ) for counting element access frequency . 
```cpp
class fa_cache_sim_t : public cache_sim_t
{
 public:
  fa_cache_sim_t(size_t ways, size_t linesz, const char* name);
  uint64_t* check_tag(uint64_t addr);
  uint64_t victimize(uint64_t addr);
 private:
  static bool cmp(uint64_t a, uint64_t b);
  std::map<uint64_t, uint64_t> tags;
  std::map<uint64_t, uint64_t> freq; // updated !!!
};
```
- update `check_tag` method : 
    - increase `freq[addr >> idx_shift]`
    - the rest is same as `ORIG`
- update `victimize` method : 
    - using `std::min_element` ( should include `algorithm` ) to find least frequncy used element

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 52.498 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 50.1563 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 50.3273 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 39.0397 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 39.0293 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 35.9197 %
=======================================================================
Data Cache Setting with: 2:1:32
Miss Rate: 35.4987 %
=======================================================================
Data Cache Setting with: 1:1:64
Miss Rate: 36.4023 %
=======================================================================
Policy: lfu
The best data cache setting is:
Setting: Set = 2, Way = 1, BlockSize = 32
Miss Rate: 35.4987
```

### LRU

For `LRU` ( Least Recently Used ) I add another member `stamp` ( which type is `std::map` ) for counting element access frequency . 
```cpp
class fa_cache_sim_t : public cache_sim_t
{
 public:
  fa_cache_sim_t(size_t ways, size_t linesz, const char* name);
  uint64_t* check_tag(uint64_t addr);
  uint64_t victimize(uint64_t addr);
 private:
  static bool cmp(uint64_t a, uint64_t b);
  std::map<uint64_t, uint64_t> tags;
  std::map<uint64_t, uint64_t> stamp; // updated !!!
  uint64_t stamp_counter = 0;         // updated !!! 
};
```
- update `check_tag` method : 
    - increase `stamp_counter` and update to `freq[addr >> idx_shift]`
    - the rest is same as `ORIG`
- update `victimize` method : 
    - using `std::min_element` ( should include `algorithm` ) to find least recently used element

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 48.898 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 50.1563 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 50.3273 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 39.0397 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 39.0293 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 35.9197 %
=======================================================================
Data Cache Setting with: 2:1:32
Miss Rate: 35.4987 %
=======================================================================
Data Cache Setting with: 1:1:64
Miss Rate: 36.4023 %
=======================================================================
Policy: lru
The best data cache setting is:
Setting: Set = 2, Way = 1, BlockSize = 32
Miss Rate: 35.4987
```

## My tools
- `ngrok`
```
ngrok tcp 22
```
- `ssh`
```
ssh USER@tcp://0.tcp.jp.ngrok.io -p PORT                            
```
<!-- ssh ubuntu@tcp://0.tcp.jp.ngrok.io -p 14864                             -->
- `rsync`
```
rsync -r -av -e "ssh SERVER_USER@0.tcp.jp.ngrok.io -p PORT" lab3 :SERVER_DIR_PATH --progress
```
<!-- rsync -r -av -e "ssh ubuntu@0.tcp.jp.ngrok.io -p 14864" lab3 :/home/ubuntu/Desktop/Jason --progress -->