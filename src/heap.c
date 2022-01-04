#include "heap.h"
#include "my_malloc.h"

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

heap *create_heap(int table_size) {
  heap *table = NULL;
  
  table = mmap(0, table_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (table == MAP_FAILED) {
    return table;
  }
  table->chunk = mmap(0, table_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (table->chunk == MAP_FAILED) {
    return table;
  }  
  table->size = table_size;
  table->nbr_chunks = 0;
  table->next = NULL;

  return table;
}

void append_new_heap(heap *table) {
  if (!head) {
    head = table;
    return;
  }

  heap *ptr = head;
  while(ptr->next) {
    ptr = ptr->next;
  }

  ptr->next = table;
}

void handle_colision(int hashed_key, struct chunks *chunk, heap *table) {
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

void free_chunk(long key, heap *table) {
  int hashed_key = hash_function(key);
  char *ptr = (char *)table->chunk[hashed_key].memory;
  
  for (int i = 0; i < table->chunk[hashed_key].c_info.size; i++) {
    ptr[i] = 0;
  }
  
  table->chunk[hashed_key].c_info.used = 0;
}

void delete_single_heap(heap **table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    (*table)->chunk[i].key = 0;
    (*table)->chunk[i].c_info.used = 0;
    (*table)->chunk[i].c_info.start = 0;
    (*table)->chunk[i].c_info.size = 0;
    (*table)->chunk[i].c_info.end = 0;
    munmap((*table)->chunk[i].memory, sizeof((*table)->chunk[i].memory));
  }
  (*table)->size = 0;
  (*table)->nbr_chunks = 0;
  (*table)->next = NULL;
  munmap((*table)->chunk, sizeof((*table)->chunk));
  munmap((*table), sizeof((*table)));
  (*table) = NULL;
}

void delete_full_heap() {
  heap **ptr = &head;
  heap *ptr_next = NULL;
  
  while ((*ptr)) {
    ptr_next = (*ptr)->next;
    delete_single_heap(ptr);
    (*ptr) = ptr_next;
  }
}
chunks *create_chunk(long key, void *memory) {
  chunks *chunk = NULL;
  chunk_info c_info; 

  c_info.used = 1;
  c_info.start = key;
  c_info.size = (int)sizeof(memory);
  c_info.end = c_info.start + c_info.size;
  
  chunk = mmap(0, sizeof(chunks), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (chunk == MAP_FAILED) {
    return NULL;
  }

  chunk->key = key;
  chunk->memory = memory;
  chunk->c_info = c_info;

  return chunk;
}

char insert_chunk_into_heap(chunks *chunk, heap *table) {
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

chunks *get_chunk_from_heap(long key, heap *table) {
  chunks *new_chunk = NULL;
  int hashed_key = hash_function(key);
  new_chunk = &table->chunk[hashed_key];

  return new_chunk;
}

char is_chunk_used(long key, heap *table) {
  int hashed_key = hash_function(key);

  return table->chunk[hashed_key].c_info.used;  
}
