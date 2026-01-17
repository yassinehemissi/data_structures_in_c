#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct Vec {
	void** items;
	size_t item_size; 
	size_t size; 
	size_t t_size; // total size 
} Vec;

Vec * vector_create(size_t item_size, size_t t_size);
void vector_items_destroy(void** items, size_t size);
void vector_destroy(Vec * v);
int vector_resize(Vec * v);
void vector_push(Vec * v, void* value);
void vector_pop(Vec * v);
void * vector_get(Vec* v, size_t index);
int vector_find(Vec* v, void * val);
int vector_delete(Vec * v, size_t index);
int vector_set(Vec * v, size_t index, void * value);
int vector_insert(Vec * v, size_t index, void * value);

#endif
