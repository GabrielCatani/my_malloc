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

int handle_colision(int hashed_key, heap *table) {
  for (int i = hashed_key; i < table->capacity; i++) {
    if (table->chunk[i].memory == NULL) {
      return i;
    }
    else if (i == (table->capacity - 1)) {
	i = -1;
    }
  }

  return -1;
}

void init_heap(int size) {
  int heap_size = 0;

  if (size >= MMAP_THRESHOLD) {
    heap_size = size;
  }
  else {
    heap_size = MMAP_THRESHOLD;
  }
  
  heap_head = mmap(0, sizeof(heap *), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  heap_head->chunk = mmap(0, sizeof(chunks *), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  //heap_head->chunk = mmap(0, sizeof(chunks) * TABLE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  heap_head->capacity = TABLE_SIZE;
  heap_head->nbr_chunks = 0;
  heap_head->memory = mmap(0, heap_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  heap_head->free_size = heap_size;
  heap_head->next = NULL;
}

heap *append_new_heap(int size) {
  heap *ptr = heap_head;
  int heap_size = 0;
  
  while (ptr->next) {
    ptr = ptr->next;
  }

  if (size >= MMAP_THRESHOLD) {
    heap_size = size;
  }
  else {
    heap_size = MMAP_THRESHOLD;
  }
  
  ptr->next = mmap(0, sizeof(heap *), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  ptr->next->chunk = mmap(0, sizeof(chunks *), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  //ptr->next->chunk = mmap(0, sizeof(chunks) * TABLE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  ptr->next->capacity = TABLE_SIZE;
  ptr->next->nbr_chunks = 0;
  ptr->next->memory = mmap(0, heap_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  ptr->next->free_size = heap_size;
  ptr->next->next = NULL;

  return ptr->next;
}

char is_heap_new(heap *begin) {
  if(!begin) {
    return -1;
  }
  if (begin &&
      begin->nbr_chunks == 0) {
    return 1;
  }

  return 0;
}

char create_first_chunk(heap *begin, int size) {
  char *ref_ptr = begin->memory + size;
  int hashed_key = 0;
  chunks new_chunk;
  chunks rest_of_heap;
  
  new_chunk.key = (long)begin->memory;
  hashed_key = hash_function(new_chunk.key);
  hashed_key = handle_colision(hashed_key, begin);
  if (hashed_key == -1) {
    return 1;
  }
  new_chunk.size = size;
  new_chunk.free = 0;
  new_chunk.memory = begin->memory;

  begin->chunk[hashed_key] = new_chunk;


  rest_of_heap.key = ((long)begin->memory + size);
  hashed_key = hash_function(rest_of_heap.key);
  hashed_key = handle_colision(hashed_key, begin);
  if (hashed_key == -1) {
    return 1;
  }
  rest_of_heap.size = begin->free_size - size;
  begin->free_size = rest_of_heap.size; 
  rest_of_heap.free = 1;
  rest_of_heap.memory = (void *)ref_ptr;

  begin->chunk[hashed_key] = rest_of_heap;
  begin->nbr_chunks = 2;

  return 0;
}

heap *find_free_heap(int size) {
  heap *ptr = heap_head;
  
  while (ptr) {
    if (ptr->free_size >= size) {
      return ptr;
    }
    ptr = ptr->next;
  }
  ptr = append_new_heap(size);
  return ptr;
}

char add_new_chunk(int size) {
  heap *ptr_heap = find_free_heap(size);
  char *ref_ptr = NULL;
  int hashed_key = 0;
  int original_size = 0;
  
  if (ptr_heap->nbr_chunks == 0) {
    create_first_chunk(ptr_heap, size);
    return 0;
  }
  else {
    for (int i = 0; i < TABLE_SIZE; i++) {
      if (ptr_heap->chunk[i].size == size &&
	  ptr_heap->chunk[i].free) {
        ptr_heap->chunk[i].free = 0;
	ptr_heap->free_size -= size;
	return 0;
      }
      else if (ptr_heap->chunk[i].size > size &&
	  ptr_heap->chunk[i].free) {
	original_size = ptr_heap->chunk[i].size;
        ptr_heap->chunk[i].free = 0;
	ptr_heap->chunk[i].size = size;

	ref_ptr = ptr_heap->chunk[i].memory;
	ref_ptr = ref_ptr + size;
	hashed_key = hash_function((long)ref_ptr);
	hashed_key = handle_colision(hashed_key, ptr_heap);

	ptr_heap->chunk[hashed_key].key = (long)ref_ptr;
	ptr_heap->chunk[hashed_key].size = original_size - size; 
	ptr_heap->chunk[hashed_key].free = 1;
	ptr_heap->chunk[hashed_key].memory = ref_ptr;
	ptr_heap->free_size -= size;
	ptr_heap->nbr_chunks += 1;	
	return 0;
      }
    }   
  }
  return 1;
}

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

  printf("\n\n===============================\n");  
  
}

void print_heap(heap *begin) {
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
      printf("Chunk key: %lu -> Size: %d -> is Free: %d -> Memory: %s\n", begin->chunk[i].key, begin->chunk[i].size, begin->chunk[i].free, begin->chunk[i].memory);
    }
  }
}

void free_chunk(void *memory) {
  char *ptr_mem = (char *)memory;
  chunks *chunk_ptr = get_chunk(memory);

  if (chunk_ptr) {
    for (int i = 0; i < chunk_ptr->size; i++) {
      ptr_mem[i] = '\0';
    }
    chunk_ptr->free = 1;    
  }  

  merge_free_chunks(chunk_ptr);
  free_heap_if_no_chunks();
}

chunks *get_chunk(void *memory) {
  int hashed_key = hash_function((long)memory);
  heap *ptr_heap = heap_head;
  
  while (ptr_heap) {
    if (ptr_heap->chunk[hashed_key].key == (long)memory) {
      return &ptr_heap->chunk[hashed_key];
    }
    else {
      for (int i = 0; i < TABLE_SIZE; i++) {
        if (ptr_heap->chunk[i].key == (long)memory) {
          return &ptr_heap->chunk[i];
	}
      }
    }
    ptr_heap = ptr_heap->next;
  }

  return NULL;
}

void merge_free_chunks(chunks *ref_chunk) {
  if (!ref_chunk) {
    return;
  }

  chunks *chunk_to_analyze = NULL;
  void *memory = NULL;
  long ref_key = ref_chunk->key;
  
  ref_key += ref_chunk->size;
  memory = (void *)ref_key;
  chunk_to_analyze = get_chunk(memory);
  while (chunk_to_analyze && chunk_to_analyze->free) {
    ref_chunk->size += chunk_to_analyze->size;
    remove_chunk(chunk_to_analyze->key);

    ref_key = ref_chunk->key;
    ref_key += ref_chunk->size;
    memory = (void *)ref_key;
    chunk_to_analyze = get_chunk(memory);
  }
}

void remove_chunk(long key) {
  int hashed_key = hash_function(key);
  heap *ptr_heap = heap_head;
  
  while (ptr_heap) {
    if (ptr_heap->chunk[hashed_key].key == key) {
      ptr_heap->chunk[hashed_key].key = 0;
      ptr_heap->chunk[hashed_key].size = 0;
      ptr_heap->chunk[hashed_key].free = 0;
      ptr_heap->chunk[hashed_key].memory = 0;
      ptr_heap->nbr_chunks--;
      return;
    }
    else {
      for (int i = 0; i < TABLE_SIZE; i++) {
        if (ptr_heap->chunk[i].key == key) {
          ptr_heap->chunk[i].key = 0;
	  ptr_heap->chunk[i].size = 0;
	  ptr_heap->chunk[i].free = 0;
	  ptr_heap->chunk[i].memory = 0;
          ptr_heap->nbr_chunks--;
          return;
	}
      }
    }
    ptr_heap = ptr_heap->next;
  }
}

void destroy_heap() {
  heap *prev = heap_head;
  int heap_size = 0;

  while (prev) {
    heap_head = heap_head->next;

    for (int i = 0; i < TABLE_SIZE; i++) {
      heap_size += prev->chunk[i].size;
    }
    
    munmap(prev->chunk, sizeof (chunks *));
    munmap(prev->memory, heap_size);
    munmap(prev, sizeof(heap *));
    heap_size = 0;
    prev = heap_head;
  }
}

void free_heap_if_no_chunks() {
  heap **prev = NULL;
  heap *current = NULL;
  int heap_size = 0;
  
  if (heap_head->nbr_chunks == 0) {
    for (int i = 0; i < heap_head->capacity; i++) {
      heap_size += heap_head->chunk[i].size;
    }
    
    munmap(heap_head->chunk, sizeof (chunks *));
    munmap(heap_head->memory, heap_size);
    current = heap_head;
    heap_head = heap_head->next;
    munmap(heap_head, sizeof(heap *));
    heap_size = 0;

    current = NULL;
    return;
  }

  prev = &heap_head;
  current = heap_head->next;
  while (current && current->nbr_chunks != 0) {
    prev = &current;
    current = current->next;
  }

  if (!current) {
    return;
  }

  for (int i = 0; i < current->capacity; i++) {
      heap_size += current->chunk[i].size;
    }
    
  munmap(current->chunk, sizeof (chunks *));
  munmap(current->memory, heap_size);
  (*prev)->next = current->next;
  munmap(current, sizeof(heap *));

  current = NULL;
}
