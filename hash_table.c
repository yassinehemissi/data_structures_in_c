#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdint.h>

typedef struct HTRecord {
	char * k;
	void * v;
	struct HTRecord* next;
} HTRecord;

typedef struct {
	HTRecord** buckets;
	size_t capacity;
} HashTable;

// Hashing Function
unsigned long hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++)) {
	    hash = ((hash << 5) + hash) + c;
	}
	return hash;
};


// Record Functions

HTRecord* htr_create(char * key, void * value){
	HTRecord * new_record = malloc(sizeof(new_record)); 
	if (!new_record) return NULL;
	new_record->k = strdup(key); 
	new_record->v = value; 
	new_record->next = NULL; 
	return new_record; 
}

void htr_delete(HTRecord * htr){
	free(htr->k);
	free(htr->v);
	free(htr); 
}

// Hash Table Function 

HashTable * ht_create(size_t capacity){
	if (capacity < 8)
		capacity = 8; 
	HashTable * ht = (HashTable*) malloc(sizeof(HashTable));
	if (!ht) return NULL; 
	ht->capacity = capacity; 
	ht->buckets = calloc(ht->capacity, sizeof(HTRecord)); 
	if (!ht->buckets){
		free(ht);
		return NULL; 
	}
	return ht; 
}


void ht_put(HashTable * ht, char * key, void * value){
	// We'll skip the capacity check here as we'll impelment a resize later on 
	uint64_t idx = hash(key) % ht->capacity; 
	HTRecord* new_record = htr_create(key, value);
	if (!ht->buckets[idx]){
		ht->buckets[idx] = new_record;
		return; 
	}
	HTRecord* e = ht->buckets[idx]; 
	while (e){
		if (strcmp(e->k, key) == 0){
			e->v = value; 
			htr_delete(new_record); 
			return; 
		}
		if (!e->next) break; 
		e = e->next; 
	}
	e->next = new_record;  
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
	if (!ht->buckets[idx]) return NULL; 
	HTRecord* e = ht->buckets[idx]; 
	HTRecord* prev = NULL; 
	while (e) {
		if (strcmp(e->k, key) == 0){
			if (prev == NULL){
				ht->buckets[idx] = e->next; 
			} else 
				prev->next = e->next;
			}
			htr_delete(e);
			return; 
		prev = e; 
		e = e->next; 
	}
}



