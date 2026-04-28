#include <stdlib.h>
#include <string.h>
#include "bitmap_index.h"

// Create bitmap index
BitmapIndex* bitmap_index_create(int32_t num_records) {
    BitmapIndex *bitmap = (BitmapIndex *)malloc(sizeof(BitmapIndex));
    bitmap->num_records = num_records;
    bitmap->bitmap_size = (num_records + 7) / 8;  // Round up to nearest byte
    bitmap->max_entries = 100;  // Initial capacity for distinct values
    bitmap->entries = (BitmapEntry *)malloc(100 * sizeof(BitmapEntry));
    bitmap->num_entries = 0;
    
    return bitmap;
}

// Helper: Set bit in bitmap
static void bitmap_set_bit(uint8_t *bitmap, int32_t pos) {
    bitmap[pos / 8] |= (1 << (pos % 8));
}

// Helper: Get bit from bitmap
static int bitmap_get_bit(uint8_t *bitmap, int32_t pos) {
    return (bitmap[pos / 8] >> (pos % 8)) & 1;
}

// Helper: Find or create bitmap entry
static BitmapEntry* bitmap_find_or_create_entry(BitmapIndex *bitmap, int32_t value) {
    // Search for existing entry
    for (int32_t i = 0; i < bitmap->num_entries; i++) {
        if (bitmap->entries[i].value == value) {
            return &bitmap->entries[i];
        }
    }
    
    // Create new entry if not found
    if (bitmap->num_entries >= bitmap->max_entries) {
        bitmap->max_entries *= 2;
        bitmap->entries = (BitmapEntry *)realloc(bitmap->entries,
                                                 bitmap->max_entries * sizeof(BitmapEntry));
    }
    
    BitmapEntry *entry = &bitmap->entries[bitmap->num_entries];
    entry->value = value;
    entry->bitmap_size = bitmap->bitmap_size;
    entry->bitmap = (uint8_t *)calloc(bitmap->bitmap_size, sizeof(uint8_t));
    bitmap->num_entries++;
    
    return entry;
}

// Build bitmap index from database
void bitmap_index_build(BitmapIndex *bitmap, Database *db) {
    for (int32_t i = 0; i < db->num_records; i++) {
        int32_t value = db->records[i].department_id;
        BitmapEntry *entry = bitmap_find_or_create_entry(bitmap, value);
        bitmap_set_bit(entry->bitmap, i);
    }
}

// Search bitmap index
QueryResult* bitmap_index_search(BitmapIndex *bitmap, Database *db, int32_t value) {
    QueryResult *result = (QueryResult *)malloc(sizeof(QueryResult));
    result->results = (Record *)malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    // Find the bitmap entry for the value
    BitmapEntry *entry = NULL;
    for (int32_t i = 0; i < bitmap->num_entries; i++) {
        if (bitmap->entries[i].value == value) {
            entry = &bitmap->entries[i];
            break;
        }
    }
    
    if (entry != NULL) {
        // Scan bitmap and collect records
        for (int32_t i = 0; i < db->num_records; i++) {
            if (bitmap_get_bit(entry->bitmap, i)) {
                result->results[result->count++] = db->records[i];
            }
        }
    }
    
    return result;
}

// Free bitmap index
void bitmap_index_free(BitmapIndex *bitmap) {
    if (bitmap) {
        for (int32_t i = 0; i < bitmap->num_entries; i++) {
            if (bitmap->entries[i].bitmap) {
                free(bitmap->entries[i].bitmap);
            }
        }
        free(bitmap->entries);
        free(bitmap);
    }
}

// Calculate memory usage
uint64_t bitmap_index_get_memory_usage(BitmapIndex *bitmap) {
    uint64_t total = sizeof(BitmapIndex);
    total += bitmap->max_entries * sizeof(BitmapEntry);
    
    for (int32_t i = 0; i < bitmap->num_entries; i++) {
        total += bitmap->entries[i].bitmap_size;
    }
    
    return total;
}
