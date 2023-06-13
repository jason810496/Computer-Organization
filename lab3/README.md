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
    - `check_tag` 
    - `victimize`
    

For implemetation , we just need to change `check_tag` and `victimize` methods ( or add other helper members ) in `cache_sim_t` with appropriate data structure depends on different cache policy .

## Implementation 
An experienced classmate , [Chumy](https://github.com/Jimmy01240397)  wrote a brute force script to enumerate all possible `Set` ,`Way` , and `BlockSize` .

Based on [original script](https://github.com/jason810496/Computer-Organization/blob/main/lab3/chumy.sh) ,  I add some code to print best miss rate result in the end of `output.txt`. ( [bruteforce.sh](https://github.com/jason810496/Computer-Organization/blob/main/lab3/bruteforce.sh) )

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

For `FIFO` ( first in first out ) I use `std::list` as a queue , and the elements record data block index.

```cpp
class cache_sim_t : public cache_sim_t
{  
    // .... the rest is all same 
    uint64_t write_misses;
    uint64_t bytes_written;
    uint64_t writebacks;

    std::string name;
    bool log;
    std::list<uint64_t> que; // updated !!!

    void init();
};
```
- update `check_tag` method : 
    - using `push_back` to add data block in queue.
- update `victimize` method : 
    - using `push_back` and `pop_back` as a FIFO queue

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 50.3037 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 60.702 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 55.9603 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 50.74 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 45.7173 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 41.975 %
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
class cache_sim_t : public cache_sim_t
{  
    // .... the rest is all same 
    uint64_t write_misses;
    uint64_t bytes_written;
    uint64_t writebacks;

    std::string name;
    bool log;
    std::map<uint64_t,uint64_t> freq; // updated !!!

    void init();
};
```
- update `check_tag` method : 
    - increase `freq[idx*ways + i]`
    - the rest is same as `ORIG`
- update `victimize` method : 
    - initialize some variables and use a for-loop to find index of LFU element .
    - update victimize block .

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 50.3037 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 50.4283 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 50.9797 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 43.3073 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 42.6497 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 38.0477 %
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

For `LRU` ( Least Recently Used ) I add another member `stamp` ( which type is `std::map` ) and `stamp_counter` ( as a timestamp ) for recording element access time . 
```cpp
class cache_sim_t : public cache_sim_t
{  
    // .... the rest is all same 
    uint64_t write_misses;
    uint64_t bytes_written;
    uint64_t writebacks;

    std::string name;
    bool log;

    std::map<uint64_t, uint64_t> stamp; // updated !!!
    uint64_t stamp_counter; // updated !!!

    void init();
};
```
- update `check_tag` method : 
    - increase `stamp_counter` and update to `stamp[idx*ways + i]`
    - the rest is same as `ORIG`
- update `victimize` method : 
    - initialize some variables and use a for-loop to find index of LFU element .
    - update victimize block .

#### Testing
```
=======================================================================
Data Cache Setting with: 1:8:8
Miss Rate: 50.3037 %
=======================================================================
Data Cache Setting with: 2:4:8
Miss Rate: 48.715 %
=======================================================================
Data Cache Setting with: 4:2:8
Miss Rate: 49.307 %
=======================================================================
Data Cache Setting with: 8:1:8
Miss Rate: 52.0297 %
=======================================================================
Data Cache Setting with: 1:4:16
Miss Rate: 38.5147 %
=======================================================================
Data Cache Setting with: 2:2:16
Miss Rate: 38.36 %
=======================================================================
Data Cache Setting with: 4:1:16
Miss Rate: 40.45 %
=======================================================================
Data Cache Setting with: 1:2:32
Miss Rate: 34.983 %
=======================================================================
Data Cache Setting with: 2:1:32
Miss Rate: 35.4987 %
=======================================================================
Data Cache Setting with: 1:1:64
Miss Rate: 36.4023 %
=======================================================================
Policy: lru
The best data cache setting is:
Setting: Set = 1, Way = 2, BlockSize = 32
Miss Rate: 34.983
```

And turn out that `LRU` with `1:2:32` is the best miss rate setting !

## Final Result 

Final `config.conf` setting :
```
[cache]
Set = 1
Way = 2
BlockSize = 32
Policy = "lru"
```
Judging :
```bash
make score
```
Result : **`miss rate` lower than 35% !!!**
```
=======================================================================
Policy: "lru"
Data Cache Setting with: 1:2:32
Miss Rate: 34.983 %
```


## My tools

My laptop doesn't have enough space to use `VMWare` to mount the required `.ova` environment . Thanks to my friend [@owen](https://github.com/owenowenisme/) lend me his laptop to finish this lab !

The following is some tools help me sync code from develop env to production dev : 

- `ngrok`
```
ngrok tcp 22
```
- `ssh`
```
ssh USER@tcp://0.tcp.jp.ngrok.io -p PORT                            
```
<!-- ssh ubuntu@tcp://0.tcp.jp.ngrok.io -p 19112                             -->
- `rsync`
```
rsync -r -av -e "ssh SERVER_USER@0.tcp.jp.ngrok.io -p PORT" lab3 :SERVER_DIR_PATH --progress
```
<!-- rsync -r -av -e "ssh ubuntu@0.tcp.jp.ngrok.io -p 19112" lab3 :/home/ubuntu/Desktop/Jason --progress -->
