#ifndef __HEAP_H__
#define __HEAP_H__
#define TABLE_SIZE  10
#include <stdio.h>
#include <sys/mman.h>
typedef struct h_table heap;
typedef struct chunk_info chunk_info;
typedef struct chunks chunks;
struct h_table {
  chunks *chunk;
  int size;
  int nbr_chunks;
  heap *next;
};

struct chunk_info {
  char used;
  long start;
  int size;
  long end;
};

struct chunks {
  long key;
  chunk_info c_info;
  void *memory;
};

int hash_function(long key);
heap *create_heap(int table_size);
void append_new_heap(int table_size);
void handle_colision(int hashed_key, chunks *chunk, heap *table);
void free_chunk(long key, heap *table);
void delete_single_heap(heap **table);
void delete_full_heap();
chunks *create_chunk(long key, void *memory);
char insert_chunk_into_heap(chunks *chunk, heap *table);
chunks *get_chunk_from_heap(long key, heap *table);
char is_chunk_used(long key, heap *table);
#endif
