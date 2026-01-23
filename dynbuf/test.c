
DynBuf b;
db_init(&b, 4);

db_append(&b, "abc", 3);
db_append(&b, "defgh", 5);

assert(b.len == 8);
assert(memcmp(b.data, "abcdefgh", 8) == 0);

db_free(&b);
