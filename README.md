# my_malloc

## Introduction ##

my_malloc is aproject to recreate my own version of the C function family of malloc (calloc, realloc)
and the function free().

## Usage ##

##### Static Library #####

      make //generates static library 'my_malloc'

To use the lib, with a program:

      gcc your_program.c -L. -lmy_malloc -I inc/ -o you_executable


##### Testing my_malloc #####

      make test

Run tests

      ./test

## Tackling the problem ##

The goal of the project is straight forward: use the mmap syscall to retrieve memory to the user.
The trick is to do it efficently, calling mmap as few times as possible. In order to do it, at each
user calls, instead of only retriveing the amount of memory asked by the user, the function will fecth a big chunk fo memory, and
manage memory (rertieving and freeing it) as the user needs it.

## Data Structures ##

#### Heap Sections ####

  Once the first mmap is called, the memory returned by it is store in a **heap section** object, which basically is the memory returned by
  mmap with a header that contains info. about that section of memory. With new calls to malloc (and/or realloc, calloc, and free),
  in case the first memory has no more space, a new **heap section** is created. The heap sections are organized in a single **linked list**.

  Each heap section is divided into chunks, that are returned to the user, and are divided into two types.

  The memory of each heap section is organized into a **hashtable**, in which each entry is a chunk.

##### Heap header #####

      chunks *chunk; // Hashtable of chunks
      int capacity; // Max number of entries of hashtable
      int nbr_chunks; // number of chunks
      void *memory; // memory section returned from mmap syscall
      int free_size; // highest free chunks size
      heap *next; // next heap_section

#### Chunks ####

  What differeciates a chunk is the header parameter 'free', that indicates if the chunks is used or free.
  Chunks are the blocks returned for the user in order to use all mallloc functions (malloc, realloc, calloc and free).

##### Chunk header #####

      long key; // key of chunk (used by hashfunction, to add chunk on hashtable
      int size; // size of memory of chunk
      char free; // free == 1 -> free chunk; free == 0 -> used chunk
      void *memory; // memory from heap section, pointed by chunk
    