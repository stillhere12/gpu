#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "benchmark.h"

#define NUM_RECORDS 50000
#define NUM_QUERIES 1000

int main() {
    printf("\n=== HPC Database Query Optimization Mini Project ===\n\n");
    
    // Create and populate database
    printf("Creating database with %d records...\n", NUM_RECORDS);
    Database *db = db_create();
    db_populate(db, NUM_RECORDS);
    printf("✓ Database created successfully\n\n");
    
    // Create and build index
    printf("Building index on department_id...\n");
    Index *idx = index_create(HASH_TABLE_SIZE);
    
    PerfTimer *index_timer = perf_timer_create();
    perf_timer_start(index_timer);
    index_build(idx, db, 0);
    perf_timer_stop(index_timer);
    printf("✓ Index built in %.4f ms\n\n", perf_timer_elapsed_ms(index_timer));
    perf_timer_free(index_timer);
    
    // Benchmark: Linear Search
    printf("=== Linear Search Benchmark ===\n");
    printf("Running %d queries with linear search...\n", NUM_QUERIES);
    
    PerfTimer *linear_timer = perf_timer_create();
    perf_timer_start(linear_timer);
    
    QueryResult *results_linear = NULL;
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        results_linear = query_linear_search(db, 0, dept_id);
        query_result_free(results_linear);
    }
    
    perf_timer_stop(linear_timer);
    double linear_time = perf_timer_elapsed_ms(linear_timer);
    printf("Total time: %.4f ms\n", linear_time);
    printf("Avg query time: %.6f ms\n\n", linear_time / NUM_QUERIES);
    perf_timer_free(linear_timer);
    
    // Benchmark: Indexed Search
    printf("=== Indexed Search Benchmark ===\n");
    printf("Running %d queries with indexed search...\n", NUM_QUERIES);
    
    PerfTimer *indexed_timer = perf_timer_create();
    perf_timer_start(indexed_timer);
    
    QueryResult *results_indexed = NULL;
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        results_indexed = query_indexed_search(idx, db, dept_id);
        query_result_free(results_indexed);
    }
    
    perf_timer_stop(indexed_timer);
    double indexed_time = perf_timer_elapsed_ms(indexed_timer);
    printf("Total time: %.4f ms\n", indexed_time);
    printf("Avg query time: %.6f ms\n\n", indexed_time / NUM_QUERIES);
    perf_timer_free(indexed_timer);
    
    // Performance comparison
    printf("=== Performance Comparison ===\n");
    double speedup = linear_time / indexed_time;
    double improvement = ((linear_time - indexed_time) / linear_time) * 100.0;
    
    printf("Speedup: %.2f x\n", speedup);
    printf("Improvement: %.2f %%\n\n", improvement);
    
    // Sample query result
    printf("=== Sample Query Result ===\n");
    QueryResult *sample = query_indexed_search(idx, db, 1);
    printf("Found %d records with department_id=1\n", sample->count);
    printf("First 3 results:\n");
    for (int i = 0; i < 3 && i < sample->count; i++) {
        printf("  ID: %d, Name: %s, Age: %d, Dept: %d, Salary: %.2f\n",
               sample->results[i].id,
               sample->results[i].name,
               sample->results[i].age,
               sample->results[i].department_id,
               sample->results[i].salary);
    }
    printf("\n");
    query_result_free(sample);
    
    // Cleanup
    index_free(idx);
    db_free(db);
    
    printf("=== Execution Complete ===\n\n");
    
    return 0;
}
