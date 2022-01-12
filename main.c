#include "my_malloc.h"
#include <unistd.h>

int main(void) {

  init_heap(10);

  heap *ptr = heap_head;
  
  add_new_chunk(5);
  add_new_chunk(3);
  add_new_chunk(2);
  add_new_chunk(10);
  
  while (ptr) {
    print_heap(ptr);
    ptr = ptr->next;
  }
  
  return 0;
}
