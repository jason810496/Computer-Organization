# Cache

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

For implemetation , we just need to change `tags` member in `fa_cache_sim_t` to appropriate data structure with different cache policy .

## Implementation 
An experienced classmate , [Chumy](https://github.com/Jimmy01240397)  wrote a brute force script ([`bruteforce.sh`]() ) to enumerate all possible `Set` ,`Way` , and `BlockSize` .

Based on original script ,  I add some code to print best miss rate result in the end of `output.txt`.
### ORIG
The original cachesim use `LFSR` ( Linear feedback shift register ) , 

#### Testing

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
    - using `std::find_if` ( should include `algorithm` ) to 

#### Testing

## LRU
Least Recentlu Use
- time stamp queue : store last access element


## My tools
- `ssh`
```
ssh USER@tcp://0.tcp.jp.ngrok.io -p PORT                            
```
<!-- ssh ubuntu@tcp://0.tcp.jp.ngrok.io -p 14864                             -->
- `ngrok` + `rsync`
```
rsync -r -av -e "ssh SERVER_USER@0.tcp.jp.ngrok.io -p PORT" lab3 :SERVER_DIR_PATH --progress
```
<!-- rsync -r -av -e "ssh ubuntu@0.tcp.jp.ngrok.io -p 14864" lab3 :/home/ubuntu/Desktop/Jason --progress -->