#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "heap.h"; 

Heap * heap_create(int htype, size_t item_size){
  Heap * h = malloc(sizeof(Heap));
  if (!h) return NULL; 
  h->htype = htype;
  h->item_size = item_size;
  h->data = vector_create(item_size, 8); // creating vectr with 8 total space and item_size  
  if (!h->data){
    free(h);
    return NULL;
  };
  return h;
}

int step_up(int htype, int cmp_r){
  if (htype && cmp_r >= 0) return 0; 
  else if (!htype && cmp_r <= 0) return 0; 
  return 1;
}

int step_down(int htype, int cmp_r){
  if (htype && cmp_r <= 0) return 0; 
  else if (!htype && cmp_r >= 0) return 0; 
  return 1;
}

void heapify(size_t index, void * value, Heap * h, int step_up){ // step_up = 1 or step_down if 0 
  size_t p_idx;
  void * parent;
  if (step_up){
    p_idx = (size_t) ceil((double) index / 2);  // parent index ceil of (position / 2) 
    parent = vector_get(h->data, p_idx);
  } else {
    void * ch1 = vector_get(index * 2 + 1);
    void * ch2 = vector_get(index * 2 + 2);
    int cmp = memcmp(ch1, ch2, h->item_size);
    if ((cmp >= 0 && h->htype) || (cmp <= 0 && !h->htype)){
      parent = ch1; 
      p_idx = index * 2 + 1; 
    } else {
      parent = ch2; 
      p_idx = index * 2 + 2; 
    }  
  } 
  int cmp_r = memcmp(value, parent, h->item_size); 
  int result; 
  size_t next_index;
  if (step_up) result = step_up(htype, cmp_r);
  else result = step_down(htype, cmp_r);
  if (!result) return; 
  h->data->items[p_idx] = h->data->items[index];
  h->data->items[index] = parent;
  heapify(p_idx, value, h);
};

int heap_push(Heap * h, void * value){
  vector_push(h->data,  value);
  heapify(h->data->size - 1, value, h, 1); 
}

int heap_pop(){
  h->data->items[0] = h->data->items[h->data->size - 1]; 
  vector_pop(h->data);
  heapify(0, h->data->items[0], h, 0);
}

void * heap_peek(Heap * h){
  return h->data->items[0];  
} 



