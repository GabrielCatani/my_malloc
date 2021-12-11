# my_malloc

## Introduction ##

my_malloc is aproject to recreate my own version of the C function family of malloc (calloc, realloc)
and the function free().

## Usage ##

### --WIP-- ###

## Tackling the problem ##

The goal of the project is straight forward: use the mmap sys/call to retrieve memory to the user.
The trick is to do it efficently, calling mmap as few times as possible. In order to do it, at each
user calls, instead of inly retriveing the amount asked, the function will fecth a big chunk fo memory, and
manage memory (rertieving and freeing it) as the user needs it.

## Data Structures ##

Once the first mmap is called, I will retrieve a big chunk of memory (greater then the user asked), and categorize this in two types:

#### Used chunks ####
   Part of the memory returned from the first call to mmap, that will be retrieved to the user, with the size requested.

#### Free chunks ###
   The rest of the memory returned from mmap (not used/asked by the user).


Wether been a used or free chunck, both categories will have the following elements:

    - **start**: starting address of chunk
    - **size**: size of chunk
    - **end**: ending address of chunk (start + size)

To keep track, and easy to add, remove and use, each category of chunk will have its own HashTable, using it start as key:


#### Hashtable chunk example ####

   ```
   HashTable free_chunks;

   free_chunks->table[0x0000000000784ab] //Fictional start of address of the refered free chunk

   /*
   [0x0000000000784ab]
   -> start = 0x0000000000784ab;
   -> size  = 8;
   -> end   = 0x0000000000784B3
   */
   ```
   