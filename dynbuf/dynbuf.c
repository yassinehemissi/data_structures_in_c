#include <stdlib.h>
#include <string.h>
#include "dynbuf.h"

int db_init(DynBuf *b, size_t initial_cap){
  if (!b) return -1;
  b->data = NULL;
  b->cap = 0;
  b->len = 0;
  if (initial_cap == 0) return 0;
  b->data = malloc(initial_cap);
  if (!b->data) return -1;
  b->cap = initial_cap;
  return 0;
}

void db_free(DynBuf *b){
  if (!b) return;
  free(b->data);
  b->data = NULL;
  b->cap = 0;
  b->len = 0;
}

int db_reserve(DynBuf *b, size_t new_cap){ 
  if (!b) return -1;
  unsigned char * tmp = realloc(b->data, new_cap);
  if (!tmp) return -1;
  b->data = tmp;
  b->cap = new_cap;
  return 0;
}

int db_append(DynBuf *b, const void *src, size_t n){
  if (!b) return -1;
  if (b->len + n >= b->cap){
    int res = db_reserve(b, b->len + n + 1);
    if (res == -1) return -1;
  }
  memcpy(b->data + b->len, src, n);
  b->len += n;
  return 0;
}
