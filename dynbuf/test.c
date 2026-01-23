#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dynbuf.h"

static void print_state(const char *label, DynBuf *b) {
    printf("[%s]\n", label);
    printf("  data: %p\n", (void*)b->data);
    printf("  len : %zu\n", b->len);
    printf("  cap : %zu\n", b->cap);
    if (b->data && b->len > 0) {
        printf("  data: \"");
        for (size_t i = 0; i < b->len; ++i)
            putchar(b->data[i]);
        printf("\"\n");
    }
    printf("\n");
}

static void test_dynbuf_stress_append_bytes(void) {
    printf("\n=== DynBuf stress: append 1 byte 100000 times ===\n");

    DynBuf b;
    assert(db_init(&b, 1) == 0);

    void *p0 = (void*)b.data;
    printf("[start] data=%p len=%zu cap=%zu\n", (void*)b.data, b.len, b.cap);

    const size_t N = 100000;

    for (size_t i = 0; i < N; ++i) {
        unsigned char c = (unsigned char)('a' + (i % 26));
        assert(db_append(&b, &c, 1) == 0);

        // Invariant must always hold
        assert(b.len <= b.cap);
    }

    printf("[end]   data=%p len=%zu cap=%zu\n", (void*)b.data, b.len, b.cap);
    printf("Pointer moved? %s\n", ((void*)b.data != p0) ? "YES (realloc moved it)" : "NO (grew in place)");

    // Semantic checks
    assert(b.len == N);
    assert(b.data[0] == 'a');
    assert(b.data[25] == 'z');
    assert(b.data[26] == 'a');
    assert(b.data[N - 1] == (unsigned char)('a' + ((N - 1) % 26)));

    db_free(&b);
    printf("=== Stress append test PASSED ===\n");
}
void test_dynbuf_simple(void) {
    DynBuf b;

    printf("=== DynBuf basic test ===\n\n");

    /* init */
    assert(db_init(&b, 4) == 0);
    print_state("after init(4)", &b);

    /* append small */
    assert(db_append(&b, "abc", 3) == 0);
    print_state("after append(\"abc\", 3)", &b);

    /* append forces grow */
    assert(db_append(&b, "defgh", 5) == 0);
    print_state("after append(\"defgh\", 5)", &b);

    /* semantic checks */
    assert(b.len == 8);
    assert(memcmp(b.data, "abcdefgh", 8) == 0);
    assert(b.len <= b.cap);

    /* append zero bytes */
    assert(db_append(&b, "", 0) == 0);
    print_state("after append(\"\", 0)", &b);

    /* free */
    db_free(&b);
    print_state("after free", &b);

    /* post-free invariants */
    assert(b.data == NULL);
    assert(b.len == 0);
    assert(b.cap == 0);
    printf("=== DynBuf test PASSED ===\n");
}

static void test_dynbuf_stress_reserve_then_append(void) {
    printf("\n=== DynBuf stress: reserve big then append ===\n");

    DynBuf b;
    assert(db_init(&b, 4) == 0);

    // Build initial content
    assert(db_append(&b, "hello", 5) == 0);
    assert(b.len == 5);
    assert(memcmp(b.data, "hello", 5) == 0);

    size_t old_cap = b.cap;
    size_t old_len = b.len;
    void *old_ptr   = (void*)b.data;

    printf("[before reserve] data=%p len=%zu cap=%zu\n", old_ptr, b.len, b.cap);

    // Reserve a lot (should not change len or content)
    size_t target = old_cap + 100000;
    assert(db_reserve(&b, target) == 0);

    printf("[after reserve]  data=%p len=%zu cap=%zu\n", (void*)b.data, b.len, b.cap);
    printf("Pointer moved? %s\n", ((void*)b.data != old_ptr) ? "YES" : "NO");

    assert(b.cap >= target);
    assert(b.len == old_len);
    assert(memcmp(b.data, "hello", 5) == 0);

    // Now append and verify combined content
    assert(db_append(&b, " world", 6) == 0);
    assert(b.len == 11);
    assert(memcmp(b.data, "hello world", 11) == 0);

    db_free(&b);
    printf("=== Stress reserve test PASSED ===\n");
}

int test_dynbuf(void) {
    test_dynbuf_simple();
    test_dynbuf_stress_append_bytes();
    test_dynbuf_stress_reserve_then_append();
    return 0;
}
