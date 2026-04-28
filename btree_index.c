#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "btree_index.h"

// Create a new B-tree node
static BTreeNode* btree_node_create(int32_t is_leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    memset(node->keys, 0, sizeof(node->keys));
    memset(node->record_counts, 0, sizeof(node->record_counts));
    
    for (int i = 0; i < BTREE_ORDER * 2 - 1; i++) {
        node->record_indices[i] = (int32_t *)malloc(100 * sizeof(int32_t));
    }
    
    for (int i = 0; i < BTREE_ORDER * 2; i++) {
        node->children[i] = NULL;
    }
    
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    return node;
}

// Insert a key into a leaf node (assumes space available)
static void btree_insert_non_full(BTreeNode *node, int32_t key, int32_t record_idx) {
    int32_t i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Find position for key
        int32_t found_pos = -1;
        
        // First check if key already exists
        for (int32_t j = 0; j < node->num_keys; j++) {
            if (node->keys[j] == key) {
                found_pos = j;
                break;
            }
        }
        
        if (found_pos >= 0) {
            // Duplicate key - add to existing
            if (node->record_counts[found_pos] < 100) {
                node->record_indices[found_pos][node->record_counts[found_pos]++] = record_idx;
            }
        } else if (node->num_keys < BTREE_ORDER * 2 - 2) {
            // New key - find insertion position
            while (i >= 0 && node->keys[i] > key) {
                node->keys[i + 1] = node->keys[i];
                memcpy(node->record_indices[i + 1], node->record_indices[i], 
                       node->record_counts[i] * sizeof(int32_t));
                node->record_counts[i + 1] = node->record_counts[i];
                i--;
            }
            
            node->keys[i + 1] = key;
            node->record_indices[i + 1][0] = record_idx;
            node->record_counts[i + 1] = 1;
            node->num_keys++;
        }
    }
}

// Simple B-tree insert (no splitting for simplicity)
static void btree_insert_simple(BTreeNode *node, int32_t key, int32_t record_idx) {
    if (node->num_keys < BTREE_ORDER * 2 - 2) {
        btree_insert_non_full(node, key, record_idx);
    }
}

// Create B-tree index
BTreeIndex* btree_index_create() {
    BTreeIndex *btree = (BTreeIndex *)malloc(sizeof(BTreeIndex));
    btree->root = btree_node_create(1);  // Root is initially a leaf
    btree->height = 1;
    return btree;
}

// Build B-tree from database
void btree_index_build(BTreeIndex *btree, Database *db) {
    for (int32_t i = 0; i < db->num_records; i++) {
        int32_t value = db->records[i].department_id;
        btree_insert_simple(btree->root, value, i);
    }
}

// Search in B-tree (linear search in leaf nodes for simplicity)
static QueryResult* btree_search_recursive(BTreeNode *node, Database *db, 
                                           int32_t value, QueryResult *result) {
    int32_t i = 0;
    
    // Find appropriate child/leaf position
    while (i < node->num_keys && value > node->keys[i]) {
        i++;
    }
    
    // If key found in current node
    if (i < node->num_keys && value == node->keys[i]) {
        for (int32_t j = 0; j < node->record_counts[i]; j++) {
            int32_t record_idx = node->record_indices[i][j];
            result->results[result->count++] = db->records[record_idx];
        }
    }
    
    // If not leaf, search in children
    if (!node->is_leaf && node->children[i] != NULL) {
        btree_search_recursive(node->children[i], db, value, result);
    }
    
    return result;
}

// Query B-tree for exact match
QueryResult* btree_index_search(BTreeIndex *btree, Database *db, int32_t value) {
    QueryResult *result = (QueryResult *)malloc(sizeof(QueryResult));
    result->results = (Record *)malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    return btree_search_recursive(btree->root, db, value, result);
}

// Free B-tree node recursively
static void btree_node_free(BTreeNode *node) {
    if (!node) return;
    
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            if (node->children[i]) {
                btree_node_free(node->children[i]);
            }
        }
    }
    
    for (int i = 0; i < BTREE_ORDER * 2 - 1; i++) {
        if (node->record_indices[i]) {
            free(node->record_indices[i]);
        }
    }
    
    free(node);
}

// Free B-tree index
void btree_index_free(BTreeIndex *btree) {
    if (btree) {
        btree_node_free(btree->root);
        free(btree);
    }
}

// Calculate memory usage
uint64_t btree_get_memory_usage(BTreeIndex *btree __attribute__((unused))) {
    // Simplified: estimate based on structure
    return sizeof(BTreeIndex) + 100 * (BTREE_ORDER * 2) * sizeof(BTreeNode);
}
