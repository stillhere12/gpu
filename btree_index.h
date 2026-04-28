#ifndef BTREE_INDEX_H
#define BTREE_INDEX_H

#include <stdint.h>
#include "database.h"

#define BTREE_ORDER 16  // Branching factor

// B-tree node
typedef struct BTreeNode {
    int32_t keys[BTREE_ORDER * 2 - 1];
    int32_t *record_indices[BTREE_ORDER * 2 - 1];  // Dynamic arrays for duplicate keys
    int32_t record_counts[BTREE_ORDER * 2 - 1];    // Count of records per key
    struct BTreeNode *children[BTREE_ORDER * 2];
    int32_t num_keys;
    int32_t is_leaf;
} BTreeNode;

// B-tree index structure
typedef struct {
    BTreeNode *root;
    int32_t height;
} BTreeIndex;

// Function declarations
BTreeIndex* btree_index_create();
void btree_index_build(BTreeIndex *btree, Database *db);
QueryResult* btree_index_search(BTreeIndex *btree, Database *db, int32_t value);
void btree_index_free(BTreeIndex *btree);
uint64_t btree_get_memory_usage(BTreeIndex *btree);

#endif // BTREE_INDEX_H
