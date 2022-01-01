#include "my_malloc.h"

int main(void) {

  //Set up for tests
  void *memory = mmap(0, sizeof(char) * 5, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  
  void *memory_2 = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  void *memory_3 = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);  
  
  h_table *table = create_hash_table(TABLE_SIZE);
  chunks *chunk = create_chunk((long)memory, memory);
  chunks *chunk_2 = create_chunk((long)memory_2, memory_2);
  chunks *chunk_3 = create_chunk((long)memory_3, memory_3);

  printf("=======Unit Tests=======\n");
  
  //Tests insert_chunk_into_table
  insert_chunk_into_table(chunk, table);
  insert_chunk_into_table(chunk_2, table);
  insert_chunk_into_table(chunk_3, table);

  int counter = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (table->chunk[i].key == chunk->key ||
	table->chunk[i].key == chunk_2->key ||
	table->chunk[i].key == chunk_3->key) {
      counter++;
    }
  }

  if (counter == 3) {
    printf("insert_chunk_into_table: \033[0;32m[OK]\033[0m\n");
  }
  else {
    printf("inser_chunk_into_table: \033[0;31m[NOK]\033[0m\n");
  }
  /*
  * Show Hash Table with chunks inserted 
  *
  printf("Table size: %d Table nbr: %d\n", table->size, table->nbr_chunks);
  for (int i = 0; i < 10; i++) {
    printf("Key: [%d] -> Element: [%ld]\n", i, table->chunk[i].key);
  }
  */

  //Test get_chunk_from_table
  chunks *got_chunk = get_chunk_from_table((long)memory, table);
  if (got_chunk->c_info.size == chunk->c_info.size &&
  got_chunk->c_info.start == chunk->c_info.start) {
    printf("get_chunk_from_table: \033[0;32m[OK]\033[0m\n");  
  }
  else {
    printf("get_chunk_from_table: \033[0;31m[NOK]\033[0m\n");
  }

  //Test free_chunk
  int hashed_key = hash_function((long)memory);
  
  free_chunk((long)memory, table);

  if (table->chunk[hashed_key].c_info.used == 0 &&
      (long)table->chunk[hashed_key].memory == 0) {
    printf("free_chunk: \033[0;32m[OK]\033[0m\n");  
  }
  else {
    printf("free_chunk: \033[0;31m[NOK]\033[0m\n");
  }

  //Test is_chunk_used
  if (!is_chunk_used(hashed_key, table)) {
    printf("chunk_used: \033[0;32m[OK]\033[0m\n");
  }
  else {
    printf("chunk_used: \033[0;31m[NOK]\033[0m\n");
  }

  //Test delete_hash_table
  delete_hash_table(table);
  munmap(table, sizeof(table));
  printf("%lu\n", sizeof(table));
  if (!table) {
    printf("delete_hash_table: \033[0;32m[OK]\033[0m\n");
  }
  else {
    printf("delete_hash_table: \033[0;31m[NOK]\033[0m\n");
  }
  
  return 0;
}
