#include "my_malloc.h"
#include <unistd.h>

int main(void) {

  init_heap(10);

  heap *ptr = heap_head;
  
  add_new_chunk(5);
  add_new_chunk(3);
  add_new_chunk(2);

  ptr = heap_head;
  while (ptr) {
    print_heap(ptr);
    ptr = ptr->next;
  }

  char *ptr_mem = NULL;
  char c = 'a';
  int chunk_index = 0;

  for (int i = 0; i < TABLE_SIZE; i++) {
    if (heap_head->chunk[i].size && !heap_head->chunk[i].free) {
      ptr_mem = heap_head->chunk[i].memory;
      chunk_index = i;
      for (int w = 0; w < heap_head->chunk[i].size; w++) {
        ptr_mem[w] = c;
	c++;
      }
      break;
    }
  }

  
  ptr = heap_head;
  while (ptr) {
    print_heap(ptr);
    ptr = ptr->next;
  }
  
  printf("\nChunk_Size: %d\n", get_chunk_size(heap_head->chunk[chunk_index].memory));
  
  free_chunk(heap_head->chunk[chunk_index].memory);

  ptr = heap_head;
  while (ptr) {
    print_heap(ptr);
    ptr = ptr->next;
  }
  
  return 0;
}
