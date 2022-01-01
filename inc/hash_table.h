#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__
#define TABLE_SIZE  10
#include <stdio.h>
#include <sys/mman.h>
typedef struct h_table h_table;
typedef struct chunk_info chunk_info;
typedef struct chunks chunks;

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

struct h_table {
  chunks *chunk;
  int size;
  int nbr_chunks;
};

int hash_function(long key);
h_table *create_hash_table(int table_size);
chunks *create_chunk(long key, void *memory);
void handle_colision(int hashed_key, struct chunks *chunk, h_table *table);
char insert_chunk_into_table(struct chunks *chunk, h_table *table);
chunks *get_chunk_from_table(long key, h_table *table);
void free_chunk(long key, h_table *table);
char is_chunk_used(long key, h_table *table);
//TODO
void delete_hash_table(h_table *table);
#endif
