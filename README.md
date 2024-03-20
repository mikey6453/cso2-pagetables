# Multi-Level Page Table Library

## Overview
This library provides an implementation of a multi-level page table for virtual memory systems. It is configurable and adaptable for various depths of page table levels and page sizes.

## Customizing `config.h`
`config.h` is a critical file for customizing the behavior of the page table. It contains two definitions:

- `LEVELS`: This represents the number of levels in the page table. Acceptable values range from 1 to 6. Adjust this based on the required depth of your page table hierarchy.
- `POBITS`: This defines the size of page offsets, indirectly dictating the size of each page table. Acceptable values range from 4 to 18. This setting also affects the number of entries in each page table.

### Choosing Values
- **LEVELS**: Increase for deeper table structures, useful for larger virtual address spaces. More levels increase the time complexity of translations and allocations.
- **POBITS**: Increase for larger page sizes. Larger pages can reduce the number of page tables but may increase memory wastage due to internal fragmentation.

## Known Limitations
- This implementation currently does not support concurrent access. In multi-threaded environments, care must be taken to ensure thread safety.
- Only tested extensively for single-level page tables. Multi-level tables may exhibit unexplored edge cases.

## Suggestions for Future Expansion
- Implement thread safety features.
- Optimize for memory usage and access speed, particularly for deeper page table levels.
- Include more comprehensive error handling and reporting mechanisms.
