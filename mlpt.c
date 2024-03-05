#include "mlpt.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define _XOPEN_SOURCE 700
#define MAX 0xFFFFFFFFFFFFFFFF
#define PAGE_SIZE (1 << POBITS)
#define ENTRY_COUNT (PAGE_SIZE / sizeof(size_t))
#define VALID_BIT_MASK 0x1

size_t ptbr = 0;

void page_allocate(size_t va) {
    if (ptbr == 0) {
        if (posix_memalign((void **)&ptbr, PAGE_SIZE, PAGE_SIZE) != 0) {
            fprintf(stderr, "Failed to allocate page table\n");
            exit(EXIT_FAILURE);
        }
        memset((void *)ptbr, 0, PAGE_SIZE);
    }
    size_t index = va >> POBITS;
    size_t *page_table = (size_t *)ptbr;
    if ((page_table[index] & VALID_BIT_MASK) == 0) {
        size_t *new_page;
        if (posix_memalign((void **)&new_page, PAGE_SIZE, PAGE_SIZE) != 0) {
            fprintf(stderr, "Failed to allocate page\n");
            exit(EXIT_FAILURE);
        }
        memset(new_page, 0, PAGE_SIZE);
        size_t ppn = ((size_t)new_page) >> POBITS;
        page_table[index] = (ppn << POBITS) | VALID_BIT_MASK;
    }
}

size_t translate(size_t va) {
    if (ptbr == 0) {
        return MAX;
    }
    size_t index = va >> POBITS;
    size_t offset = va & (PAGE_SIZE - 1);
    size_t *page_table = (size_t *)ptbr;
    size_t entry = page_table[index];
    if (entry & VALID_BIT_MASK) {
        size_t ppn = entry >> POBITS;
        return (ppn << POBITS) + offset;
    } else {
        return MAX;
    }
}
