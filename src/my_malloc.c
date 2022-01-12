#include "my_malloc.h"


//TODO
void *malloc(size_t size) {
  //check if there is any heap, if not, create new heap
  //Iterate over heap_head
    //Iterate over each chunk of heap
     /* check if chunk_info is free
     *    if free chunk_info
     *      allocate memory in chunk
     *      'move' rest of chunk to new_chunk:
     *       (new_chunk->start = allocated_chunk->end)
     *       merge_free_chunks
     *       return *memory of chunk allocated
     *
     * If reach here (no free memory found large enough
     *  on previous heap), create new heap
     *      allocate memory in chunk
     *      'move' rest of chunk to new_chunk:
     *       (new_chunk->start = allocated_chunk->end)
     *       merge_free_chunks
     *       return *memory of chunk allocated
     *
     */
  size = 30;
  return 0;
}

//TODO
/*
void *calloc(size_t count, size_t size) {

}

void *realloc(void *ptr, size_t size) {

}

void free(void *ptr) {

}
*/
