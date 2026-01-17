#ifndef HEAP_H
#define HEAP_H

#include "../vector/vector.h"; 
#include <stddef.h>   

typedef struct Heap {
  int htype; // 0 min heap | 1 max heap  
  Vec* data; 
  size_t item_size;
} Heap; 

Heap * heap_create(int htype);
int heap_push(void * value);
int heap_pop();
int heap_peek(); 


#endif 
