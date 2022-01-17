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
 * (BIG) If size >= MMAP_THRESHOLD: heap_size will be == size
 * (NOT THAT BIG) Else heap_size will be == MMAP_THRESHOLD;
 */
void init_heap(int size);
heap *append_new_heap(int size);
//
char is_heap_new(heap *begin);
char create_first_chunk(heap *begin, int size);
// Check for free heap, or create one new heap
heap *find_free_heap(int size);
//TODO: return new created chunk
char add_new_chunk(int size);
void print_chunk(chunks *chunk);
void print_heap(heap *begin);
void print_detailed_heap(heap *begin);
void free_chunk(void *memory);
chunks *get_chunk(void *memory);
void merge_free_chunks(chunks *ref_chunk);
/*
 * TODO: if free chunk result in heap with nbr_chunks == 0, free 
 * the entire heap
*/
void remove_chunk(long key);
#endif
