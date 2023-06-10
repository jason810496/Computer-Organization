# Cache

## Reference source code 
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
## LRU
Least Recentlu Use
- time stamp queue : store last access element