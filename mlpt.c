#include "mlpt.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define MAX 0xFFFFFFFFFFFFFFFF
#define PAGE_SIZE (1 << POBITS)
#define ENTRY_COUNT (PAGE_SIZE / sizeof(size_t))
#define VALID_BIT_MASK 0x1
#define INDEX_MASK ((1 << (POBITS - 3)) - 1)
#define INDEX_SHIFT(LEVEL) (POBITS + (LEVEL - 1) * (POBITS - 3))

size_t ptbr = 0;

size_t *get_or_create_page_table(size_t *current_pt, size_t index) {
    if (!(current_pt[index] & VALID_BIT_MASK)) {
        size_t *new_pt;
        if (posix_memalign((void **)&new_pt, PAGE_SIZE, PAGE_SIZE) != 0) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        memset(new_pt, 0, PAGE_SIZE);
        size_t ppn = ((size_t)new_pt) >> POBITS;
        current_pt[index] = (ppn << POBITS) | VALID_BIT_MASK;
    }
    return (size_t *)(current_pt[index] & ~VALID_BIT_MASK);
}

void page_allocate(size_t va) {
    if (!ptbr) {
        if (posix_memalign((void **)&ptbr, PAGE_SIZE, PAGE_SIZE) != 0) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        memset((void *)ptbr, 0, PAGE_SIZE);
    }

    size_t *current_pt = (size_t *)ptbr;
    for (int level = 1; level <= LEVELS; ++level) {
        size_t index = (va >> INDEX_SHIFT(level)) & INDEX_MASK;
        if (level == LEVELS) {
            if (!(current_pt[index] & VALID_BIT_MASK)) {
                size_t *new_page;
                if (posix_memalign((void **)&new_page, PAGE_SIZE, PAGE_SIZE) != 0) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                memset(new_page, 0, PAGE_SIZE);
                size_t ppn = ((size_t)new_page) >> POBITS;
                current_pt[index] = (ppn << POBITS) | VALID_BIT_MASK;
            }
            break;
        } else {
            current_pt = get_or_create_page_table(current_pt, index);
        }
    }
}

size_t translate(size_t va) {
    if (!ptbr) {
        return MAX;
    }

    size_t *current_pt = (size_t *)ptbr;
    for (int level = 1; level <= LEVELS; ++level) {
        size_t index = (va >> INDEX_SHIFT(level)) & INDEX_MASK;
        size_t entry = current_pt[index];
        if (!(entry & VALID_BIT_MASK)) {
            return MAX;
        }
        if (level == LEVELS) {
            size_t offset = va & (PAGE_SIZE - 1);
            return ((entry >> POBITS) << POBITS) | offset;
        }
        current_pt = (size_t *)(entry & ~VALID_BIT_MASK);
    }

    return MAX; // Should not reach here, added to avoid compiler warnings
}
