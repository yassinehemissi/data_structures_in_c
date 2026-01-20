// test_dijkstra.c  (C99+; uses %zu)
// AI Written Test - fixed version
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "dijkstra.h"

/*
  Assumptions based on your current project:
  - dijkstra_create_node(id, neighbors[], weights[], n) allocates a DijNode
  - dijkstra_create(source_node, total_nodes) allocates a Dijkstra*
  - dijkstra_add_node(d, node) registers nodes by id
  - dijkstra_compute(d) runs Dijkstra and fills d->distances and d->previous
  - dijkstra_destroy(d) frees:
      * heap pairs inside d->open
      * all nodes in d->nodes[] (ownership = Dijkstra)
      * d->nodes, d->previous, d->visited, d->distances, and d itself
*/

static void print_path(size_t src,
                       size_t dst,
                       const uint64_t *dist,
                       const size_t *previous,
                       size_t max_nodes)
{
    if (!dist || dist[dst] == UINT64_MAX) {
        printf("path %zu -> %zu: unreachable\n", src, dst);
        return;
    }

    size_t *stack = (size_t *)malloc(sizeof(size_t) * max_nodes);
    if (!stack) return;

    size_t top = 0;
    size_t cur = dst;

    for (size_t steps = 0; steps < max_nodes; steps++) {
        stack[top++] = cur;
        if (cur == src) break;
        cur = previous[cur];
    }

    if (stack[top - 1] != src) {
        printf("path %zu -> %zu: unreachable\n", src, dst);
        free(stack);
        return;
    }

    printf("path %zu -> %zu: ", src, dst);
    for (size_t i = 0; i < top; i++) {
        size_t idx = top - 1 - i;
        printf("%zu", stack[idx]);
        if (idx != 0) printf(" -> ");
    }
    printf("\n");

    free(stack);
}

static void test_compute_case1(void) {
    // Graph (directed):
    // 0 -> 1 (4), 0 -> 2 (1)
    // 2 -> 1 (2), 2 -> 3 (5)
    // 1 -> 3 (1)
    // 3 -> 4 (3)
    // Expected from 0: dist = [0,3,1,4,7]
    const size_t N = 5;

    size_t n0_nei[] = {1, 2};
    uint64_t n0_w[]  = {4, 1};

    size_t n1_nei[] = {3};
    uint64_t n1_w[]  = {1};

    size_t n2_nei[] = {1, 3};
    uint64_t n2_w[]  = {2, 5};

    size_t n3_nei[] = {4};
    uint64_t n3_w[]  = {3};

    // For n=0 nodes, arrays are unused but must be valid pointers for the call.
    size_t dummy_nei[] = {0};
    uint64_t dummy_w[] = {0};

    DijNode *node0 = dijkstra_create_node(0, n0_nei, n0_w, 2);
    DijNode *node1 = dijkstra_create_node(1, n1_nei, n1_w, 1);
    DijNode *node2 = dijkstra_create_node(2, n2_nei, n2_w, 2);
    DijNode *node3 = dijkstra_create_node(3, n3_nei, n3_w, 1);
    DijNode *node4 = dijkstra_create_node(4, dummy_nei, dummy_w, 0);

    assert(node0 && node1 && node2 && node3 && node4);

    Dijkstra *d = dijkstra_create(node0, N);
    assert(d);

    dijkstra_add_node(d, node1);
    dijkstra_add_node(d, node2);
    dijkstra_add_node(d, node3);
    dijkstra_add_node(d, node4);

    dijkstra_compute(d);

    printf("\n[case1] Distances from source %zu:\n", d->source);
    for (size_t i = 0; i < N; i++) {
        printf("  dist[%zu] = ", i);
        if (d->distances[i] == UINT64_MAX) printf("INF\n");
        else printf("%llu\n", (unsigned long long)d->distances[i]);
    }

    assert(d->distances[0] == 0);
    assert(d->distances[2] == 1);
    assert(d->distances[1] == 3);
    assert(d->distances[3] == 4);
    assert(d->distances[4] == 7);

    print_path(0, 4, d->distances, d->previous, N);

    dijkstra_destroy(d);

    printf("[case1] OK\n");
}

static void test_compute_case2_unreachable(void) {
    // Graph (directed):
    // 0 -> 1 (2)
    // 1 -> 2 (2)
    // Node 3 is isolated (unreachable from 0)
    // Expected: dist[3] == UINT64_MAX
    const size_t N = 4;

    size_t n0_nei[] = {1};
    uint64_t n0_w[]  = {2};

    size_t n1_nei[] = {2};
    uint64_t n1_w[]  = {2};

    size_t dummy_nei[] = {0};
    uint64_t dummy_w[] = {0};

    DijNode *node0 = dijkstra_create_node(0, n0_nei, n0_w, 1);
    DijNode *node1 = dijkstra_create_node(1, n1_nei, n1_w, 1);
    DijNode *node2 = dijkstra_create_node(2, dummy_nei, dummy_w, 0);
    DijNode *node3 = dijkstra_create_node(3, dummy_nei, dummy_w, 0);

    assert(node0 && node1 && node2 && node3);

    Dijkstra *d = dijkstra_create(node0, N);
    assert(d);

    dijkstra_add_node(d, node1);
    dijkstra_add_node(d, node2);
    dijkstra_add_node(d, node3);

    dijkstra_compute(d);

    printf("\n[case2] Distances from source %zu:\n", d->source);
    for (size_t i = 0; i < N; i++) {
        printf("  dist[%zu] = ", i);
        if (d->distances[i] == UINT64_MAX) printf("INF\n");
        else printf("%llu\n", (unsigned long long)d->distances[i]);
    }

    assert(d->distances[0] == 0);
    assert(d->distances[1] == 2);
    assert(d->distances[2] == 4);
    assert(d->distances[3] == UINT64_MAX);

    print_path(0, 2, d->distances, d->previous, N); // reachable
    print_path(0, 3, d->distances, d->previous, N); // unreachable

    dijkstra_destroy(d);

    printf("[case2] OK\n");
}

// Call this from your runner or main()
int test_dijkstra(void) {
    test_compute_case1();
    test_compute_case2_unreachable();
    printf("\n[tests] ALL OK\n");
    return 0;
}

