#include "visualize_memory.h"

void print_chunk(chunks *chunk) {
  printf("==========CHUNK Header==========\n");
  printf("Chunk Key: %lu\nSize: %d\n", chunk->key, chunk->size);
  
  if (chunk->free) {
    printf("Is Free: Yes\n");
  }
  else {
    printf("Is Free: No\n");
  }
  
  for (int i = 0; i < chunk->size; i++) {
    if (chunk->free) {
      printf("\033[0;32m0\033[0m");
    }
    else {
      printf("\033[0;31m0\033[0m");
    }
  }

  printf("\n");
  char *ptr = (char *)chunk->memory;
  for (int i = 0; i < chunk->size; i++) {
    printf("%d ", ptr[i]);
  }

  printf("\n\n===============================\n");  
  
}

void print_heap_section(heap *begin) {
  printf("==========HEAP Header==========\n");
  if (!begin) {
    return;
  }
  printf("HEAP Free Space: %d\nNBR of Chunks: %d\n\n", begin->free_size, begin->nbr_chunks);

  int heap_size = 0;
  
  for (int i = 0; i < TABLE_SIZE; i++) {
    heap_size += begin->chunk[i].size;
  }  

  long mem_ref = (long)begin->memory;
  while (begin) {
    for (int i = 0; i < heap_size; i++) {
      for (int w = 0; w < begin->capacity; w++) {
        if (begin->chunk[w].key == mem_ref) {
	  if (begin->chunk[w].free) {
            printf("\033[0;32m[ %d ]\033[0m", begin->chunk[w].size);
	  }
	  else {
            printf("\033[0;31m[ %d ]\033[0m", begin->chunk[w].size);
	  }
	  mem_ref += begin->chunk[w].size;
          break;
	}
      } 
    }
   
    begin = begin->next;
  }

  printf("\n\n===============================\n");    
}

void print_full_heap() {
  heap *ptr = heap_head;

  while (ptr) {
    print_heap_section(ptr);
    ptr = ptr->next;
  }
}

void print_detailed_heap(heap *begin) {
  //Print Header
  printf("==========HEAP Header==========\n");
  printf("HEAP Free Space: %d\nNBR of Chunks: %d\n\n", begin->free_size, begin->nbr_chunks);

  //Print Visual representation
  int heap_size = 0;
  long mem_ref = (long)begin->memory;
  int chunk_index = 0;
  
  for (int i = 0; i < TABLE_SIZE; i++) {
    heap_size += begin->chunk[i].size;
  }
  
  int index = 0;
  while (index < heap_size) {
    for (int i = 0; i < TABLE_SIZE; i++) {
      if (begin->chunk[i].key == mem_ref) {
	chunk_index = i;
	break;
      }
    }

    for (int w = 0; w < begin->chunk[chunk_index].size; w++) {
      if (begin->chunk[chunk_index].free) {
        printf("\033[0;32m0\033[0m");
      }
      else {
        printf("\033[0;31m0\033[0m");
      }
      index++;
      mem_ref++;
    }
    printf(" | ");
  }
  printf("\n\n===============================\n");
  
  //Print Chunks details
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (begin->chunk[i].size != 0) {
      printf("Chunk key: %lu -> Size: %d -> is Free: %d -> Memory: %lu\n", begin->chunk[i].key, begin->chunk[i].size, begin->chunk[i].free, (long)begin->chunk[i].memory);
    }
  }
}
