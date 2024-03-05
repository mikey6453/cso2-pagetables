#ifndef MLPT_H
#define MLPT_H

#include <stddef.h>

extern size_t ptbr;

size_t translate(size_t va);
void page_allocate(size_t va);

#endif 
