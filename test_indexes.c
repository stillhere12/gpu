#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "btree_index.h"
#include "hash_index.h"
#include "bitmap_index.h"

int main() {
    printf("Testing index implementations...\n\n");
    
    // Create small test database
    Database *db = db_create();
    db_populate(db, 100);
    printf("Created database with 100 records\n");
    printf("Department IDs: 1-10 (each appearing ~10 times)\n\n");
    
    // Test Hash Index
    printf("=== Hash Index Test ===\n");
    HashIndex *hash = hash_index_create(10007);
    hash_index_build(hash, db);
    QueryResult *hash_result = hash_index_search(hash, db, 1);
    printf("Hash index search for dept_id=1: found %d records\n", hash_result->count);
    query_result_free(hash_result);
    hash_index_free(hash);
    
    // Test B-tree Index
    printf("\n=== B-tree Index Test ===\n");
    BTreeIndex *btree = btree_index_create();
    btree_index_build(btree, db);
    printf("B-tree root has %d keys\n", btree->root->num_keys);
    QueryResult *btree_result = btree_index_search(btree, db, 1);
    printf("B-tree search for dept_id=1: found %d records\n", btree_result->count);
    printf("First result id: %d\n", btree_result->count > 0 ? btree_result->results[0].id : -1);
    query_result_free(btree_result);
    btree_index_free(btree);
    
    // Test Bitmap Index
    printf("\n=== Bitmap Index Test ===\n");
    BitmapIndex *bitmap = bitmap_index_create(db->num_records);
    bitmap_index_build(bitmap, db);
    printf("Bitmap has %d entries (distinct values)\n", bitmap->num_entries);
    QueryResult *bitmap_result = bitmap_index_search(bitmap, db, 1);
    printf("Bitmap search for dept_id=1: found %d records\n", bitmap_result->count);
    query_result_free(bitmap_result);
    bitmap_index_free(bitmap);
    
    // Cleanup
    db_free(db);
    printf("\n✓ All tests completed\n");
    
    return 0;
}
