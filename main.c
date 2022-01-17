#include "my_malloc.h"
#include <unistd.h>

int main(void) {

  init_heap(10);
  
  add_new_chunk(5);
  add_new_chunk(3);
  add_new_chunk(2);
  add_new_chunk(150);
  append_new_heap(250);
  
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

  heap *head_ptr = heap_head;
  while (head_ptr) {
    print_heap(head_ptr);
    head_ptr = head_ptr->next;
  }
  printf("\n\n------------------------------\n\n");
  
  free_heap();

  head_ptr = heap_head;
  
  while (head_ptr) {
    print_heap(head_ptr);
    head_ptr = head_ptr->next;
  }

  return 0;
}
