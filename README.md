# README for Multi-Level Page Table Simulation

## Configuration

Edit `config.h` to change the `LEVELS` and `POBITS` values according to your needs. The current setup is for a simple 1-level page table with 12-bit page offsets.

## Building

Run `make` to build the library `libmlpt.a` which includes the implementation for `translate` and `page_allocate` functions.

## Known Limitations

- This implementation is designed for a single level of page table (`LEVELS=1`) and 12-bit page offsets (`POBITS=12`). Adjustments are needed for multi-level support.

## How to Use

Include `mlpt.h` in your project and link against `libmlpt.a` to use the page table simulation.
