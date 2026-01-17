#ifndef HASHTABLE_H 
#define HASHTABLE_H
#include <stddef.h>

typedef struct HTRecord {
	char * k;
	void * v;
	struct HTRecord* next;
} HTRecord;

typedef struct {
	HTRecord** buckets;
	size_t capacity;
	size_t trecords; // Total Records 
} HashTable;

HTRecord* htr_create(char * key, void * value);
void htr_delete(HTRecord * htr, int freeV);
HashTable * ht_create(size_t capacity);
int ht_buckets_put(HTRecord** buckets, HTRecord* new_record, uint64_t idx);
void ht_resize(HashTable * ht);
void ht_put(HashTable * ht, char * key, void * value);	
void * ht_get(HashTable * ht, char * key);
void ht_delete(HashTable * ht, char * key);
void ht_destroy(HashTable * ht);


#endif 
