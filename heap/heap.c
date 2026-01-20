#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "heap.h" 

int step_up(size_t index, Heap * h){
  if (index == 0) return -1; 
  size_t p_idx = (index - 1) / 2; 
  int cmp_r = h->cmp(vector_get(h->data, p_idx), vector_get(h->data, index)); 
  if (h->htype && cmp_r >= 0) return -1; 
  else if (!h->htype && cmp_r <= 0) return -1;
  void * parent = vector_get(h->data, p_idx);
  h->data->items[p_idx] = h->data->items[index];
  h->data->items[index] = parent;
  return p_idx;
}

int step_down(size_t index, Heap * h){
  if (h->data->size <= index * 2 + 1) return -1;
  void * ch1 = vector_get(h->data, index * 2 + 1);
  void * ch2 = vector_get(h->data, index * 2 + 2);
  if (!ch2 && !ch1) return -1;
  void * child; 
  size_t c_index; 
  if (ch1 && ch2){
    int cmp = h->cmp(ch1, ch2);
    if ((h->htype && cmp <= 0) || (!h->htype && cmp >= 0)){
      c_index = index * 2 + 2;
      child = ch2;
    } else {
      c_index = index * 2 + 1;
      child = ch1;
    }
  } else if (ch1) {
      c_index = index * 2 + 1;
      child = ch1;
  } else if (ch2){
      c_index = index * 2 + 2;
      child = ch2;
  } 
  void * parent = vector_get(h->data, index);
  int cmp_r = h->cmp(parent, child);
  if (h->htype && cmp_r >= 0) return -1;
  else if (!h->htype && cmp_r <= 0) return -1; 
  h->data->items[index] = h->data->items[c_index];
  h->data->items[c_index] = parent;
  return c_index;
}

void heapify(size_t index, Heap * h, int is_step_up){ // step_up = 1 or step_down if 0 
  int step_result;
  if (is_step_up) step_result = step_up(index, h); 
  else step_result = step_down(index, h);
  if (step_result == -1) return; 
  heapify(step_result, h, is_step_up);
}

size_t heap_size(Heap * h){
  return h->data->size;
}

int heap_push(Heap * h, void * value){
  vector_push(h->data,  value);
  heapify(h->data->size - 1, h, 1);
  return 0;
}

Heap * heap_create(int htype, size_t item_size, HeapCompare cmp){
  Heap * h = malloc(sizeof(Heap));
  if (!h) return NULL; 
  h->htype = htype;
  h->cmp = cmp;
  h->item_size = item_size;
  h->data = vector_create(item_size, 8); // creating vectr with 8 total space and item_size  
  if (!h->data){
    free(h);
    return NULL;
  };
  return h;
}

// O(n)
Heap * heap_build(Vec * v, int htype, HeapCompare cmp){
  Heap * h = heap_create(htype, v->item_size, cmp);
  if (!h) return NULL;
  vector_destroy(h->data);
  h->data = vector_create(v->item_size, v->t_size);
  if (!h->data) {
    free(h);
    return NULL;
  };
  for (size_t i = 0; i < v->size; i++){
    vector_push(h->data, vector_get(v, i));
  };
  if (h->data->size <= 1) return h;
  for (size_t i = ((h->data->size / 2) - 1); i > 0; i--){
    heapify(i, h, 0);
  };
  heapify(0, h, 0);
  return h;
}

int heap_pop(Heap * h){
  if (h->data->size == 0) return 1;
  h->data->items[0] = h->data->items[h->data->size - 1]; 
  h->data->items[h->data->size - 1] = NULL; 
  vector_pop(h->data);
  heapify(0, h, 0);
  return 0;
}

void * heap_peek(Heap * h){
  if (h->data->size == 0) return NULL;
  return h->data->items[0];  
} 

void heap_destroy(Heap * h){
  vector_destroy(h->data);
  free(h);
}

