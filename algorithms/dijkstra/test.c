#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "dijkstra.h"

static void print_path(size_t src, size_t dst, size_t *previous, size_t max_nodes) {
    size_t *stack = malloc(sizeof(size_t) * max_nodes);
    if (!stack) return;

    size_t top = 0;
    size_t cur = dst;

    for (size_t steps = 0; steps < max_nodes; steps++) {
        stack[top++] = cur;
        if (cur == src) break;
        cur = previous[cur];
    }

    if (stack[top - 1] != src) {
        printf("path %lu -> %lu: unreachable\n",
               (unsigned long)src, (unsigned long)dst);
        free(stack);
        return;
    }

    printf("path %lu -> %lu: ",
           (unsigned long)src, (unsigned long)dst);

    for (size_t i = 0; i < top; i++) {
        size_t idx = top - 1 - i;
        printf("%lu", (unsigned long)stack[idx]);
        if (idx != 0) printf(" -> ");
    }
    printf("\n");

    free(stack);
}

void test_dijkstra(void) {
    const size_t N = 5;

    size_t n0_nei[] = {1, 2};
    uint64_t n0_w[] = {4, 1};

    size_t n1_nei[] = {3};
    uint64_t n1_w[] = {1};

    size_t n2_nei[] = {1, 3};
    uint64_t n2_w[] = {2, 5};

    size_t n3_nei[] = {4};
    uint64_t n3_w[] = {3};

    size_t dummy_nei[] = {0};
    uint64_t dummy_w[] = {0};

    DijNode *n0 = dijkstra_create_node(0, n0_nei, n0_w, 2);
    DijNode *n1 = dijkstra_create_node(1, n1_nei, n1_w, 1);
    DijNode *n2 = dijkstra_create_node(2, n2_nei, n2_w, 2);
    DijNode *n3 = dijkstra_create_node(3, n3_nei, n3_w, 1);
    DijNode *n4 = dijkstra_create_node(4, dummy_nei, dummy_w, 0);

    assert(n0 && n1 && n2 && n3 && n4);

    
    Dijkstra *d = dijkstra_create(n0, N);
    assert(d);
    dijkstra_add_node(d, n1);
    dijkstra_add_node(d, n2);
    dijkstra_add_node(d, n3);
    dijkstra_add_node(d, n4);

    dijkstra_compute(d);

    printf("Distances from source %lu:\n", (unsigned long)d->source);
    for (size_t i = 0; i < N; i++) {
        printf("dist[%lu] = %lu\n",
               (unsigned long)i,
               (unsigned long )d->distances[i]);
    }

    assert(d->distances[0] == 0);
    assert(d->distances[2] == 1);
    assert(d->distances[1] == 3);
    assert(d->distances[3] == 4);
    assert(d->distances[4] == 7);

    print_path(0, 4, d->previous, N);

    printf("\n[test_dijkstra] OK\n");
}
