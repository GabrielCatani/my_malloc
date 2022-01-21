#include "my_malloc.h"

void *malloc(size_t size) {
  //check if there is any heap, if not, create new heap
  if (!heap_head) {
    init_heap(size);
  }
  
  return add_new_chunk(size);
}

void *calloc(size_t count, size_t size) {
  int continuos_size = count * size;
  void *first_object = NULL;
  void *ref_mem = NULL;

  if (!heap_head) {
    init_heap(continuos_size);
  }

  for (size_t i = 0; i < count; i++) {
    ref_mem = add_new_chunk(size);
    my_memset(ref_mem, 0, size);
    if (i == 0) {
      first_object = ref_mem;
    }
  }

  return first_object;
}

void *realloc(void *ptr, size_t size) {
  if (!ptr) {
    return malloc(size);
  }
  
  if (ptr && size == 0) {
    free(ptr);
    return malloc(1);
  }

  if (enlarge_allocation(ptr, size)) {
    return ptr;
  }

  chunks *new_ptr = NULL;
  chunks *current_chunk = get_chunk(ptr);

  new_ptr = malloc(size);

  char *src_ptr = (char *)current_chunk->memory;
  char *dst_ptr = (char *)get_chunk(new_ptr)->memory;
  
  for (int i = 0; i < current_chunk->size; i++) {
    dst_ptr[i] = src_ptr[i];
  }
  
  
  free(ptr);
  
  return new_ptr;
}

void free(void *ptr) {
  free_chunk(ptr);
}

