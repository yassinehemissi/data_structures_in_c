#ifndef HEAP_H
#define HEAP_H

#include "../vector/vector.h" 
#include <stddef.h>   

typedef int (*HeapCompare) (const void* , const void*);

typedef struct Heap {
  int htype; // 0 min heap | 1 max heap  
  Vec* data; 
  size_t item_size;
  HeapCompare cmp;
} Heap; 

Heap * heap_create(int htype, size_t item_size, HeapCompare cmp);
int heap_push(Heap * h, void * value);
int heap_pop(Heap * h);
void * heap_peek(Heap * h); 
void heap_destroy(Heap * h);
size_t heap_size(Heap * h);


#endif 
