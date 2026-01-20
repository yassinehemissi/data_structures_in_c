#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include <stddef.h>
#include "../../heap/heap.h" // vector included 

typedef struct DijNode {
  size_t id; 
  size_t  * neighbors;
  size_t n; // count of neighbors;
  uint64_t * distances; 
} DijNode;

typedef struct DijPair {
  uint64_t d;
  size_t id;
} DijPair;

typedef struct Dijkstra {  
  Heap * open;
  size_t total_nodes;
  size_t source;  
  DijNode ** nodes; 
  bool * visited;
  size_t * previous; 
  uint64_t * distances; 
} Dijkstra;


DijNode * dijkstra_create_node(size_t id, size_t  neighbors[], uint64_t distances[], size_t n);
Dijkstra * dijkstra_create(DijNode * source, size_t total_nodes);
Dijkstra * dijkstra_add_node(Dijkstra * d, DijNode * node); 
Dijkstra * dijkstra_compute(Dijkstra * d);


#endif 
