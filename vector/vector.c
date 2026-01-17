#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "vector.h"


Vec * vector_create(size_t item_size, size_t t_size){
	if (t_size < 8) t_size = 8;
	Vec *new_vector = malloc(sizeof(Vec));
	if (!new_vector) return NULL; 
	new_vector->item_size = item_size;
	new_vector->size = 0;
	new_vector->t_size = t_size; 
	new_vector->items = calloc(new_vector->t_size , sizeof(void*));
	if (!new_vector->items){
		free(new_vector);
		return NULL; 
	}
	return new_vector;
}

void vector_items_destroy(void** items, size_t size){
	for (size_t i = 0; i < size; i++)
		if (items[i]) free(items[i]); 
	free(items);
}

void vector_destroy(Vec * v){
	vector_items_destroy(v->items, v->size); 
	free(v); 
}

int vector_resize(Vec * v) {
	size_t ot_size = v->t_size;
	v->t_size *= 2;
	void** new_items = realloc(v->items, v->t_size * sizeof(void*));
	if (new_items != NULL) {
		v->items = new_items; 
		for (size_t i = ot_size; i < v->t_size; i++)
			v->items[i] = NULL; 
	} else {
		return 1; 
	}
	return 0; 
}

void vector_push(Vec * v, void* value){
	if (v->size >= v->t_size){
		int error = vector_resize(v);
		if (error) return;
	};
	void * new_item = malloc(v->item_size);
	if (new_item == NULL) return;
	v->items[v->size] = new_item; 
	memcpy(v->items[v->size], value, v->item_size);
	(v->size)++;
}

void vector_pop(Vec * v){
	if (v->size == 0) return; 
	size_t idx = v->size - 1;
	free(v->items[idx]);
	v->items[idx] = NULL;
	v->size--; 
} 

void * vector_get(Vec* v, size_t index){
	if (index >= v->size) return NULL; 
	return v->items[index];
}

int vector_find(Vec* v, void * val){
	for (size_t i = 0; i < v->size; i++)
		if (memcmp(val, v->items[i], v->item_size) == 0) return (int) i; 	
	return -1;
}

int vector_delete(Vec * v, size_t index){
	if (index >= v->size) return -1;  
	free(v->items[index]);
	for (size_t i = index; i < v->size - 1; i++){
		v->items[i] = v->items[i + 1]; 
	}
	v->size--;
	v->items[v->size] = NULL; 
	return 0; 
}

int vector_set(Vec * v, size_t index, void * value){
	if (index >= v->size) return 1; 
	void * new_item = malloc(v->item_size);	
	if (new_item == NULL) return 1; 
	free(v->items[index]);
	v->items[index] = new_item;
	memcpy(v->items[index], value, v->item_size);
	return 0;
}

// This allows inserting only inside the interval [0, size] 
int vector_insert(Vec * v, size_t index, void * value){
	if (index > v->size) return 1;
	if (v->size >= v->t_size){
		int error = vector_resize(v);
		if (error) return 1;
	};
	void * new_item = malloc(v->item_size);
	if (new_item == NULL) return 1;
	for (size_t i = v->size; i > index; i--){
		v->items[i] = v->items[i - 1];
	};
	v->items[index] = new_item;
 	memcpy(v->items[index], value, v->item_size);
	v->size++;
	return 0;
}
