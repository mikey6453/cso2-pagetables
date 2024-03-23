#include "mlpt.h"
#include "config.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

size_t ptbr = 0;

const size_t invalidAdd= ~0b0;

size_t translate(size_t virtualAddress){
    // Check if Page Table Base Register (PTBR) is not set
    if (ptbr == 0){
        return invalidAdd; // Return invalid address if PTBR is not initialized
    }

    // Calculate offset from virtual address
    size_t offset = ((1 << POBITS) - 1) & virtualAddress;

    // Start with the base address of the page table
    size_t currentAddress = ptbr;

    // Iterate through each level of the page table
    for (int level = 0; level < LEVELS; level += 1){
        // Shift right to get the index for the current level and mask the unnecessary bits
        size_t index = virtualAddress;
        int shiftAmount = POBITS + (POBITS - 3) * (LEVELS - level - 1);

        for (int i = 0; i < shiftAmount; ++i) {
            index >>= 1;
        }

        unsigned mask = (1 << (POBITS - 3)) - 1;
        size_t vpn = index & mask;

        size_t address = currentAddress + (vpn * 8);
        size_t* pageTableEntry = (size_t*)address;

        size_t validBit = *pageTableEntry & 1;

        if (validBit == 1){
            // move to the next page table
            if (level != LEVELS - 1){
                size_t nextPageTable = (*pageTableEntry >> POBITS) << POBITS;
                currentAddress = nextPageTable;
            } else {
                // calc and return physical addres at final level
                size_t finalPageTableEntry = (*pageTableEntry >> POBITS) << POBITS;
                return finalPageTableEntry + offset;
            }
        } else {
            // Invalid bit, return invalid address
            return invalidAdd;
        }
    }
    // In case we exit the loop without returning, still return invalid address
    return invalidAdd;
}

void page_allocate(size_t virtualAddress){
    // Calculate page size using POBITS
    size_t pageSize = pow(2, POBITS);

    // Initialize PTBR
    if (ptbr == 0){
        posix_memalign((void **)(&ptbr), pageSize, pageSize); 
        memset((void *)ptbr, 0, pageSize);
    }

    size_t currentAddress = ptbr;

    // Loop through each level of multi-level page table
    for (int level = 0; level < LEVELS; level += 1){
        // Shift right to get index for current level and hide unnecesary bits
        size_t index = virtualAddress;
        int shiftAmount = POBITS + (POBITS - 3) * (LEVELS - level - 1);
        for (int i = 0; i < shiftAmount; ++i) {
            index >>= 1;
        }
        unsigned mask = (1 << (POBITS - 3)) - 1;
        size_t vpn = index & mask;

        // Calculate address 
        size_t address = currentAddress + (vpn * 8); 
        size_t* pageTableEntry = (size_t*)address; 

        size_t validBit = *pageTableEntry & 1;
        if (validBit == 1){

            currentAddress = (*pageTableEntry >> POBITS) << POBITS;
        } else {
            // If entry is not valid, create a new page table
            size_t newPageTable;
            posix_memalign((void **)(&newPageTable), pageSize, pageSize); 
            memset((void *)newPageTable, 0, pageSize); 
            newPageTable |= 1; 
            *pageTableEntry = newPageTable; 
            currentAddress = (*pageTableEntry >> POBITS) << POBITS; 
        }
    }
}
