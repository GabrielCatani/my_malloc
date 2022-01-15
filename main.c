#include "my_malloc.h"
#include <unistd.h>

int main(void) {

  init_heap(10);
  
  print_heap(heap_head);  
  add_new_chunk(5);
  print_heap(heap_head);
  add_new_chunk(3);
  add_new_chunk(2);
  add_new_chunk(150);

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
  /*
  heap *ptr = heap_head;
  while (ptr) {
    for (int i = 0; i < TABLE_SIZE; i++) {
      if (heap_head->chunk[i].size > 0) {
        print_chunk(&heap_head->chunk[i]);
      }
    }
    ptr = ptr->next;
  }
  */
  print_heap(heap_head);
  free_chunk(heap_head->chunk[chunk_index].memory);
  print_heap(heap_head);
  /*
  ptr = heap_head;
  while (ptr) {
    for (int i = 0; i < TABLE_SIZE; i++) {
      if (heap_head->chunk[i].size > 0) {
        print_chunk(&heap_head->chunk[i]);
      }
    }    
    ptr = ptr->next;
  }
  */
  
  return 0;
}
