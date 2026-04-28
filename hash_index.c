#include <stdlib.h>
#include <string.h>
#include "hash_index.h"

// Hash function
static int32_t hash_func(int32_t key, int32_t size) {
    return ((key * 2654435761U) ^ (key >> 16)) % size;
}

// Create hash index
HashIndex* hash_index_create(int32_t size) {
    HashIndex *hash = (HashIndex *)malloc(sizeof(HashIndex));
    hash->size = size;
    hash->buckets = (HashIndexEntry **)calloc(size, sizeof(HashIndexEntry *));
    hash->num_entries = 0;
    return hash;
}

// Build hash index from database
void hash_index_build(HashIndex *hash, Database *db) {
    for (int32_t i = 0; i < db->num_records; i++) {
        int32_t value = db->records[i].department_id;
        int32_t bucket = hash_func(value, hash->size);
        
        // Linear probing through chain
        HashIndexEntry *entry = hash->buckets[bucket];
        HashIndexEntry *prev = NULL;
        
        // Find if key exists in chain
        while (entry != NULL && entry->key != value) {
            prev = entry;
            entry = entry->next;
        }
        
        if (entry != NULL) {
            // Key exists, add record index
            if (entry->count >= entry->capacity) {
                entry->capacity *= 2;
                entry->record_indices = (int32_t *)realloc(entry->record_indices,
                                                           entry->capacity * sizeof(int32_t));
            }
            entry->record_indices[entry->count++] = i;
        } else {
            // New key, create entry
            entry = (HashIndexEntry *)malloc(sizeof(HashIndexEntry));
            entry->key = value;
            entry->capacity = 10;
            entry->record_indices = (int32_t *)malloc(10 * sizeof(int32_t));
            entry->record_indices[0] = i;
            entry->count = 1;
            entry->next = NULL;
            
            // Add to chain
            if (prev == NULL) {
                hash->buckets[bucket] = entry;
            } else {
                prev->next = entry;
            }
            
            hash->num_entries++;
        }
    }
}

// Search hash index
QueryResult* hash_index_search(HashIndex *hash, Database *db, int32_t value) {
    QueryResult *result = (QueryResult *)malloc(sizeof(QueryResult));
    result->results = (Record *)malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    int32_t bucket = hash_func(value, hash->size);
    HashIndexEntry *entry = hash->buckets[bucket];
    
    // Search through chain
    while (entry != NULL) {
        if (entry->key == value) {
            for (int32_t i = 0; i < entry->count; i++) {
                int32_t record_idx = entry->record_indices[i];
                result->results[result->count++] = db->records[record_idx];
            }
            break;
        }
        entry = entry->next;
    }
    
    return result;
}

// Free hash index
void hash_index_free(HashIndex *hash) {
    if (hash) {
        for (int32_t i = 0; i < hash->size; i++) {
            HashIndexEntry *entry = hash->buckets[i];
            while (entry != NULL) {
                HashIndexEntry *next = entry->next;
                if (entry->record_indices) {
                    free(entry->record_indices);
                }
                free(entry);
                entry = next;
            }
        }
        free(hash->buckets);
        free(hash);
    }
}

// Calculate memory usage
uint64_t hash_index_get_memory_usage(HashIndex *hash) {
    uint64_t total = sizeof(HashIndex);
    total += hash->size * sizeof(HashIndexEntry *);
    
    for (int32_t i = 0; i < hash->size; i++) {
        HashIndexEntry *entry = hash->buckets[i];
        while (entry != NULL) {
            total += sizeof(HashIndexEntry);
            total += entry->capacity * sizeof(int32_t);
            entry = entry->next;
        }
    }
    
    return total;
}
