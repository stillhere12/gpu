#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "database.h"

// Hash function
static int32_t hash_function(int32_t key, int32_t size) {
    return ((key * 2654435761U) ^ (key >> 16)) % size;
}

// Create database
Database* db_create() {
    Database *db = (Database *)malloc(sizeof(Database));
    db->num_records = 0;
    return db;
}

// Populate database with sample records
void db_populate(Database *db, int32_t num_records) {
    for (int32_t i = 0; i < num_records; i++) {
        db->records[i].id = i + 1;
        snprintf(db->records[i].name, MAX_NAME_LEN, "Employee_%d", i);
        db->records[i].age = 25 + (i % 40);
        db->records[i].department_id = (i % 10) + 1;
        db->records[i].salary = 50000.0f + (float)((i * 100) % 100000);
    }
    db->num_records = num_records;
}

// Free database
void db_free(Database *db) {
    if (db) {
        free(db);
    }
}

// Create index
Index* index_create(int32_t size) {
    Index *idx = (Index *)malloc(sizeof(Index));
    idx->size = size;
    idx->hash_table = (IndexEntry *)calloc(size, sizeof(IndexEntry));
    
    for (int32_t i = 0; i < size; i++) {
        idx->hash_table[i].capacity = 10;
        idx->hash_table[i].record_indices = (int32_t *)malloc(10 * sizeof(int32_t));
        idx->hash_table[i].count = 0;
    }
    
    return idx;
}

// Build index on specific field
void index_build(Index *idx, Database *db, int32_t field_offset __attribute__((unused))) {
    // For department_id field (offset 16 bytes: 4 for id, 50 for name)
    for (int32_t i = 0; i < db->num_records; i++) {
        int32_t value = db->records[i].department_id;
        int32_t hash = hash_function(value, idx->size);
        
        IndexEntry *entry = &idx->hash_table[hash];
        
        if (entry->count >= entry->capacity) {
            entry->capacity *= 2;
            entry->record_indices = (int32_t *)realloc(entry->record_indices, 
                                                       entry->capacity * sizeof(int32_t));
        }
        
        entry->record_indices[entry->count++] = i;
        entry->key = value;
    }
}

// Free index
void index_free(Index *idx) {
    if (idx) {
        for (int32_t i = 0; i < idx->size; i++) {
            if (idx->hash_table[i].record_indices) {
                free(idx->hash_table[i].record_indices);
            }
        }
        free(idx->hash_table);
        free(idx);
    }
}

// Linear search (unoptimized)
QueryResult* query_linear_search(Database *db, int32_t field_offset __attribute__((unused)), int32_t value) {
    QueryResult *result = (QueryResult *)malloc(sizeof(QueryResult));
    result->results = (Record *)malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    for (int32_t i = 0; i < db->num_records; i++) {
        if (db->records[i].department_id == value) {
            result->results[result->count++] = db->records[i];
        }
    }
    
    return result;
}

// Indexed search (optimized)
QueryResult* query_indexed_search(Index *idx, Database *db, int32_t value) {
    QueryResult *result = (QueryResult *)malloc(sizeof(QueryResult));
    result->results = (Record *)malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    int32_t hash = hash_function(value, idx->size);
    IndexEntry *entry = &idx->hash_table[hash];
    
    for (int32_t i = 0; i < entry->count; i++) {
        int32_t record_idx = entry->record_indices[i];
        if (db->records[record_idx].department_id == value) {
            result->results[result->count++] = db->records[record_idx];
        }
    }
    
    return result;
}

// Free query result
void query_result_free(QueryResult *result) {
    if (result) {
        if (result->results) {
            free(result->results);
        }
        free(result);
    }
}
