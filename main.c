#include "my_malloc.h"
#include <unistd.h>

int main(void) {

  char *ptr = malloc(128000);
  
  print_full_heap(heap_head);

  
  printf("\n\n------------------------------\n\n");

  ptr = realloc(ptr, 129000);

  print_full_heap(heap_head);


  return 0;
}
