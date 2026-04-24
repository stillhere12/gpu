#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>
#include <time.h>

#define MAX_RECORDS 100000
#define MAX_NAME_LEN 50
#define HASH_TABLE_SIZE 10007

// Record structure
typedef struct {
    int32_t id;
    char name[MAX_NAME_LEN];
    int32_t age;
    int32_t department_id;
    float salary;
} Record;

// Database structure
typedef struct {
    Record records[MAX_RECORDS];
    int32_t num_records;
} Database;

// Index entry for hash table
typedef struct {
    int32_t key;
    int32_t *record_indices;
    int32_t count;
    int32_t capacity;
} IndexEntry;

// Index structure
typedef struct {
    IndexEntry *hash_table;
    int32_t size;
} Index;

// Query result
typedef struct {
    Record *results;
    int32_t count;
} QueryResult;

// Function declarations
Database* db_create();
void db_populate(Database *db, int32_t num_records);
void db_free(Database *db);

Index* index_create(int32_t size);
void index_build(Index *idx, Database *db, int32_t field_offset);
void index_free(Index *idx);

QueryResult* query_linear_search(Database *db, int32_t field_offset, int32_t value);
QueryResult* query_indexed_search(Index *idx, Database *db, int32_t value);
void query_result_free(QueryResult *result);

#endif // DATABASE_H
