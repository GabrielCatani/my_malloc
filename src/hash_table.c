#include "hash_table.h"

int hash_function(long key) {
  int hash = 0;

  if (key == 0) {
    return 0;
  }
  while (key > 0) {
    hash += key % 10;
    key /= 10;
  }

  hash %= TABLE_SIZE;
  return hash;
}

h_table *create_hash_table(int table_size) {
  h_table *table = NULL;

  table = mmap(0, table_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  table->chunk = mmap(0, table_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);;
  table->size = table_size;
  table->nbr_chunks = 0;

  return table;
}

chunks *create_chunk(long key, void *memory) {
  chunks *chunk = NULL;
  chunk_info c_info; 

  c_info.used = 1;
  c_info.start = key;
  c_info.size = (int)sizeof(memory);
  c_info.end = c_info.start + c_info.size;
  
  chunk = mmap(0, sizeof(chunks), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  chunk->key = key;
  chunk->memory = memory;
  chunk->c_info = c_info;

  return chunk;
}

void handle_colision(int hashed_key, struct chunks *chunk, h_table *table) {
  for (int i = hashed_key; i < table->size; i++) {
    if (table->chunk[i].memory == NULL) {
      table->chunk[i] = *chunk;
      return;
    }
    else if (i == (table->size - 1)) {
	i = -1;
    }
  }
}

char insert_chunk_into_table(struct chunks *chunk, h_table *table) {
  //check if table is not full
  if (table->size == table->nbr_chunks) {
    return 1;
  }
  //hash key from chunk
  int hashed_key = 0;
  hashed_key = hash_function(chunk->key);
  //handle colision
  if (table->chunk[hashed_key].memory != NULL) {
    handle_colision(hashed_key, chunk, table);
  }
  else {
    table->chunk[hashed_key] = *chunk;  
  }

  //add nbr_chunks
  table->nbr_chunks++;

  return 0;
}

chunks *get_chunk_from_table(long key, h_table *table) {
  chunks *new_chunk = NULL;
  int hashed_key = hash_function(key);
  new_chunk = &table->chunk[hashed_key];

  return new_chunk;
}

void free_chunk(long key, h_table *table) {
  int hashed_key = hash_function(key);

  table->chunk[hashed_key].memory = 0;
  table->chunk[hashed_key].c_info.used = 0;
}

char is_chunk_used(long key, h_table *table) {
  int hashed_key = hash_function(key);

  return table->chunk[hashed_key].c_info.used;  
}

//TODO: understand how unmap works
void delete_hash_table(h_table *table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    table->chunk[i].key = 0;
    table->chunk[i].c_info.used = 0;
    table->chunk[i].c_info.start = 0;
    table->chunk[i].c_info.size = 0;
    table->chunk[i].c_info.end = 0;
    table->chunk[i].memory = 0;
  }
  table->size = 0;
  table->nbr_chunks = 0;
  munmap(table, sizeof(table));
  table = NULL;
}
