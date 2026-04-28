#ifndef BITMAP_INDEX_H
#define BITMAP_INDEX_H

#include <stdint.h>
#include "database.h"

// Bitmap index entry (for each distinct value)
typedef struct {
    int32_t value;
    uint8_t *bitmap;         // Bitmap for record positions
    int32_t bitmap_size;     // Size in bytes
} BitmapEntry;

// Bitmap index structure
typedef struct {
    BitmapEntry *entries;
    int32_t num_entries;
    int32_t max_entries;
    int32_t num_records;     // Total records (for bitmap size)
    int32_t bitmap_size;     // Bitmap size in bytes
} BitmapIndex;

// Function declarations
BitmapIndex* bitmap_index_create(int32_t num_records);
void bitmap_index_build(BitmapIndex *bitmap, Database *db);
QueryResult* bitmap_index_search(BitmapIndex *bitmap, Database *db, int32_t value);
void bitmap_index_free(BitmapIndex *bitmap);
uint64_t bitmap_index_get_memory_usage(BitmapIndex *bitmap);

#endif // BITMAP_INDEX_H
