#ifndef __VISUALIZE_MEMORY_H__
#define __VISUALIZE_MEMORY_H__
#include "heap.h"
#include "my_malloc.h"
void print_chunk(chunks *chunk);
void print_heap_section(heap *begin);
void print_full_heap();
void print_detailed_heap(heap *begin);
#endif
