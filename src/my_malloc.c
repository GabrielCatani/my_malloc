#include "my_malloc.h"
#include <stdlib.h>

void init_heap() {
  m1 = (heap *)malloc(sizeof(heap));
  m1->size = 10;
}

void delete_this_heap() {
  free(m1);
  m1 = NULL;
}
