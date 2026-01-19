#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdint.h>
#include "hash_table.h"

// Hashing Function
unsigned long hash(const char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++)) {
	    hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

// Record Functions
HTRecord* htr_create(char * key, void * value, size_t item_size){
	HTRecord * new_record = malloc(sizeof(HTRecord)); 
	if (!new_record) return NULL;
  new_record->item_size = item_size;
  new_record->v = malloc(item_size);
	if (!new_record->v) {
    free(new_record);
    return NULL; 
  }
  new_record->k = strdup(key);
  if (!new_record->k){
    free(new_record->v);
    free(new_record);
    return NULL; 
  }
	memcpy(new_record->v , value, item_size); 
	new_record->next = NULL; 
	return new_record; 
}

void htr_delete(HTRecord * htr, int freeV){
	free(htr->k);
	if (freeV) free(htr->v);
	free(htr); 
}

// Hash Table Function 
HashTable * ht_create(size_t capacity){
	if (capacity < 8)
		capacity = 8; 
	HashTable * ht = (HashTable*) malloc(sizeof(HashTable));
	if (!ht) return NULL; 
	ht->capacity = capacity; 
	ht->buckets = calloc(ht->capacity, sizeof(HTRecord*)); 
	ht->trecords = 0; 
	if (!ht->buckets){
		free(ht);
		return NULL; 
	}
	return ht; 
}

int ht_buckets_put(HTRecord** buckets, HTRecord* new_record, uint64_t idx) {
	HTRecord* e = buckets[idx];
	if (!buckets[idx]){
		buckets[idx] = new_record; 
		return 1; 
	};
	while (e){
		if (strcmp(e->k, new_record->k) == 0){
			free(e->v); 
			e->v = new_record->v;
      e->item_size = new_record->item_size;
			htr_delete(new_record, 0); 
			return 0; 
		}
		if (!e->next) break; 
		e = e->next; 
	}
	e->next = new_record;
	return 1;
}

void ht_resize(HashTable * ht){
	size_t new_capacity = ht->capacity * 2; 
	HTRecord** new_buckets = calloc(new_capacity, sizeof(HTRecord*)); 
	for (size_t i = 0; i < ht->capacity; i++){
		while (ht->buckets[i]){
			HTRecord * e = ht->buckets[i]; 
			ht->buckets[i] = e->next; 
			e->next = NULL;
			uint64_t idx = hash(e->k) % new_capacity; 	
			ht_buckets_put(new_buckets, e, idx);
		}
	}; 
	ht->capacity = new_capacity;
	free(ht->buckets);
	ht->buckets = new_buckets; 
}

void ht_put(HashTable * ht, char * key, void * value, size_t value_size){
	// Checking load factor threshold 75% which is total records / capacity  
	size_t load_factor = (ht->trecords * 100) / (ht->capacity); 
	if (load_factor > 75) ht_resize(ht); 
  
	uint64_t idx = hash(key) % ht->capacity; 
	HTRecord* new_record = htr_create(key, value, value_size);
	if (!new_record){
    return NULL;  
  }
  int result = ht_buckets_put(ht->buckets, new_record, idx); 
	// result = 0: update; 1: new  
	if (result) ht->trecords++; 
}

void * ht_get(HashTable * ht, char * key){
	uint64_t idx = hash(key) % ht->capacity; 
	if (!ht->buckets[idx]) return NULL; 
	HTRecord* e = ht->buckets[idx]; 
	while (e) {
		if (strcmp(e->k, key) == 0)
			return e->v; 
		e = e->next; 
	}
	return NULL; 
}

void ht_delete(HashTable * ht, char * key){
	uint64_t idx = hash(key) % ht->capacity; 
	if (!ht->buckets[idx]) return; 
	HTRecord* e = ht->buckets[idx]; 
	HTRecord* prev = NULL; 
	while (e) {
		if (strcmp(e->k, key) == 0){
			if (prev == NULL){
				ht->buckets[idx] = e->next; 
			} else {
				prev->next = e->next;
			}
			htr_delete(e, 1);
			ht->trecords--;
			return;
		}
		prev = e; 
		e = e->next; 
	}
}

void ht_destroy(HashTable * ht){
	for (size_t i = 0; i < ht->capacity; i++){
		if (!ht->buckets[i]) continue; 
		while (ht->buckets[i]){
			HTRecord * e = ht->buckets[i]; 
			ht->buckets[i] = e->next; 
			htr_delete(e, 1); 
		}
	}
	free(ht->buckets);
	free(ht);
}
