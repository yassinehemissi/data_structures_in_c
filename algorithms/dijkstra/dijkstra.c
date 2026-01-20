#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include "dijkstra.h"


DijNode * dijkstra_create_node(size_t id, size_t  neighbors[], uint64_t distances[], size_t n){
  DijNode * new_node = malloc(sizeof(DijNode));
  if (!new_node) return NULL;
  new_node->id = id;
  new_node->n = n;
  new_node->neighbors = malloc(sizeof(size_t) * n);
  if (!new_node->neighbors){
    free(new_node);
    return NULL;
  };
  new_node->distances = malloc(sizeof(uint64_t) * n);
  if (!new_node->distances){
    free(new_node->neighbors);
    free(new_node);
    return NULL; 
  };
  for (size_t i = 0; i < n; i++){
    new_node->distances[i] = distances[i];
    new_node->neighbors[i] = neighbors[i];
  }
  return new_node;
}

DijPair * dijkstra_create_dijpair(uint64_t d, size_t id){
  DijPair * dp = malloc(sizeof(DijPair));
  if (!dp) return NULL;
  dp->d = d;
  dp->id = id;
  return dp; 
}

int dijkstra_node_compare(const void * _a, const void * _b){
  const DijPair * a = (const DijPair *)_a; 
  const DijPair * b = (const DijPair *)_b;
  if (a->d> b->d) return 1;
  else if (a->d < b->d) return -1;
  else return 0;
}

Dijkstra * dijkstra_create(DijNode * source, size_t total_nodes) {
  Dijkstra * dij = malloc(sizeof(Dijkstra));
  if (!dij) return NULL;
  dij->total_nodes = total_nodes; 
  dij->source = source->id;  
  dij->open = heap_create(0, sizeof(DijPair), dijkstra_node_compare);
  if (!dij->open){
    free(dij);
    return NULL; 
  };
  dij->nodes = malloc(sizeof(DijNode*) * total_nodes); 
  if (!dij->nodes) {
    free(dij->open);
    free(dij);
  };
  dij->distances = malloc(total_nodes * sizeof(uint64_t)); 
  if (!dij->distances) {
    free(dij->open);
    free(dij->nodes);
    free(dij);
  };
  dij->visited = malloc(total_nodes * sizeof(bool)); 
  if (!dij->distances) {
    free(dij->open);
    free(dij->nodes);
    free(dij->distances);
    free(dij);
  };
  dij->previous = malloc(total_nodes * sizeof(size_t));
  if (!dij->previous) {
    free(dij->open);
    free(dij->nodes);
    free(dij->visited);
    free(dij->distances);
    free(dij);
  };
  for (size_t i = 0; i < dij->total_nodes; i++){
    dij->visited[i] = false;
    dij->distances[i] = UINT64_MAX;
    dij->nodes[i] = NULL;
  }
  dij->nodes[source->id] = source;

  return dij;
}

Dijkstra * dijkstra_add_node(Dijkstra * d, DijNode * node){
  if (node->id >= d->total_nodes) return d;
  d->nodes[node->id] = node;
  return d;
} 

Dijkstra * dijkstra_compute(Dijkstra * d){
  d->distances[d->source] = 0;
  DijPair * dp = dijkstra_create_dijpair(0, d->source);
  
  if (!dp) return d;
  heap_push(d->open, dp);
  while (d->open->data->size){
    DijPair * curp = d->open->data->items[0]; //heap_peek(d->open);
    if (!curp){
      heap_pop(d->open);
      continue;
    }
    
    DijNode * cur = d->nodes[curp->id];
    if (!cur) {
      free(curp);
      heap_pop(d->open);
      continue;
    }
    if (d->visited[cur->id]) {
      free(curp);
      heap_pop(d->open);
      continue;
    }
    for (size_t i = 0 ; i < cur->n; i++){
      size_t neighbor = cur->neighbors[i];
      uint64_t distance = cur->distances[i];
      if (distance + curp->d < d->distances[neighbor]){
        d->distances[neighbor] = curp->d + distance;
        d->previous[neighbor] = curp->id;
      }
      if (!(d->distances[neighbor] == curp->d + distance)) continue;
      DijPair * new_dp = dijkstra_create_dijpair(d->distances[neighbor], neighbor );
      if (!new_dp) continue;
      heap_push(d->open, new_dp);
    }
    d->visited[cur->id] = true;
    free(curp);
    heap_pop(d->open);
  } 
  return d; 
}

void dijkstra_destroy_node(DijNode * dn){
  if (!dn) return;
  free(dn->neighbors);
  free(dn->distances);
  free(dn);
}

void dijkstra_destroy(Dijkstra * d){
  if (d->open && d->open->data && d->open->data->items){
    for (size_t i = 0; i < d->open->data->size; i++)
      free(d->open->data->items[i]);
    heap_destroy(d->open);
  }
  for (size_t i = 0; i < d->total_nodes; i++)
    if (d->nodes[i]) dijkstra_destroy_node(d->nodes[i]);
  free(d->nodes);
  free(d->previous);
  free(d->visited);
  free(d->distances);
  free(d);
}
