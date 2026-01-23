#ifndef DYNBUF_H
#define DYNBUF_H

#include <stddef.h>   

typedef struct {
    unsigned char *data; 
    size_t len;           
    size_t cap;          
} DynBuf;

int  db_init(DynBuf *b, size_t initial_cap);
void db_free(DynBuf *b);
int  db_reserve(DynBuf *b, size_t new_cap);
int  db_append(DynBuf *b, const void *src, size_t n);

#endif
