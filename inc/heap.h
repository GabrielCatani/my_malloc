#ifndef __HEAP_H__
#define __HEAP_H__
#define TABLE_SIZE  10
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
void init_heap(int size);
heap *append_new_heap(int size);
char is_heap_new(heap *begin);
char create_first_chunk(heap *begin, int size);
heap *find_free_heap(int size);
char add_new_chunk(int size);
void print_heap(heap *begin);

//New heap, always with size * TABLE_SIZE
#endif
