#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__
#include "heap.h"
#include <unistd.h>
#include "visualize_memory.h"
void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
heap *heap_head;
#endif
