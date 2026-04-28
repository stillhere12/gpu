#ifndef INDEX_INTERFACE_H
#define INDEX_INTERFACE_H

#include <stdint.h>
#include "database.h"

// Index types
typedef enum {
    INDEX_TYPE_HASH,
    INDEX_TYPE_BTREE,
    INDEX_TYPE_BITMAP
} IndexType;

// Generic index structure
typedef struct {
    IndexType type;
    void *impl;  // Pointer to actual implementation (different for each type)
} GenericIndex;

// Function pointers for index operations
typedef struct {
    void (*build)(void *impl, Database *db);
    QueryResult* (*search)(void *impl, Database *db, int32_t value);
    void (*free_func)(void *impl);
    uint64_t (*get_memory_usage)(void *impl);
} IndexOps;

#endif // INDEX_INTERFACE_H
