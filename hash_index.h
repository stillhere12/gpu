#ifndef HASH_INDEX_H
#define HASH_INDEX_H

#include <stdint.h>
#include "database.h"

#define HASH_INDEX_SIZE 10007

// Hash index entry (forward declaration)
typedef struct HashIndexEntry {
    int32_t key;
    int32_t *record_indices;
    int32_t count;
    int32_t capacity;
    struct HashIndexEntry *next;  // For chaining
} HashIndexEntry;

// Hash index structure
typedef struct {
    HashIndexEntry **buckets;
    int32_t size;
    int32_t num_entries;
} HashIndex;

// Function declarations
HashIndex* hash_index_create(int32_t size);
void hash_index_build(HashIndex *hash, Database *db);
QueryResult* hash_index_search(HashIndex *hash, Database *db, int32_t value);
void hash_index_free(HashIndex *hash);
uint64_t hash_index_get_memory_usage(HashIndex *hash);

#endif // HASH_INDEX_H
