#include "my_malloc.h"
#include <unistd.h>

int main(void) {

  calloc(2, 10);
  
  char *ptr_mem = NULL;
  char c = 'a';

  for (int i = 0; i < TABLE_SIZE; i++) {
    if (heap_head->chunk[i].size && !heap_head->chunk[i].free) {
      ptr_mem = heap_head->chunk[i].memory;
      for (int w = 0; w < heap_head->chunk[i].size; w++) {
        ptr_mem[w] = c;
	c++;
      }
      break;
    }
  }

  print_heap(heap_head);
  printf("\n\n------------------------------\n\n");
  
  //destroy_heap();
  //free_heap_if_no_chunks();
  print_chunk(get_chunk(heap_head->memory));
  free(heap_head->memory);
  
  print_heap(heap_head);

  return 0;
}
