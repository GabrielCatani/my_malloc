#ifndef __HEAP_H__
#define __HEAP_H__
#define TABLE_SIZE  128
#define MMAP_THRESHOLD 128000
#include <stdio.h>
#include <sys/mman.h>
typedef struct heap heap;
typedef struct chunks chunks;
struct heap {
  chunks *chunk;
  int capacity;
  int nbr_chunks;
  void *memory;
  int free_size;
  heap *next;
};

struct chunks {
  long key;
  int size;
  char free;
  void *memory;
};
extern heap *heap_head;
int hash_function(long key);
int handle_colision(int hashed_key, heap *table);
/*
 * If heap SMALL: size * TABLE_SIZE
 * If heap BIG: create a new heap size = MMAP_THRESHOLD
 */
void init_heap(int size);
heap *append_new_heap(int size);
//
char is_heap_new(heap *begin);
char create_first_chunk(heap *begin, int size);
heap *find_free_heap(int size);
char add_new_chunk(int size);
void print_chunk(chunks *chunk);
void print_heap(heap *begin);
void print_detailed_heap(heap *begin);
void free_chunk(void *memory);
int get_chunk_size(void *memory);
#endif
