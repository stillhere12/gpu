#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "benchmark.h"
#include "btree_index.h"
#include "hash_index.h"
#include "bitmap_index.h"

#define NUM_RECORDS 100000
#define NUM_QUERIES 10000
#define NUM_ITERATIONS 3

// Structure to hold benchmark results
typedef struct {
    char name[50];
    double build_time_ms;
    double search_time_ms;
    double avg_query_time_ms;
    uint64_t memory_usage;
    int32_t result_count;
} BenchmarkResult;

// Print separator line
void print_separator() {
    printf("================================================================================\n");
}

// Linear search (baseline)
BenchmarkResult benchmark_linear_search(Database *db) {
    BenchmarkResult result;
    strcpy(result.name, "Linear Search (Baseline)");
    result.build_time_ms = 0.0;  // No index to build
    
    PerfTimer *timer = perf_timer_create();
    perf_timer_start(timer);
    
    QueryResult *last_result = NULL;
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        if (last_result) query_result_free(last_result);
        last_result = query_linear_search(db, 0, dept_id);
    }
    
    perf_timer_stop(timer);
    result.search_time_ms = perf_timer_elapsed_ms(timer);
    result.avg_query_time_ms = result.search_time_ms / NUM_QUERIES;
    result.memory_usage = 0;
    result.result_count = last_result ? last_result->count : 0;
    
    if (last_result) query_result_free(last_result);
    perf_timer_free(timer);
    
    return result;
}

// Hash index benchmark
BenchmarkResult benchmark_hash_index(Database *db) {
    BenchmarkResult result;
    strcpy(result.name, "Hash Index");
    
    // Build phase
    HashIndex *hash = hash_index_create(HASH_INDEX_SIZE);
    PerfTimer *build_timer = perf_timer_create();
    perf_timer_start(build_timer);
    hash_index_build(hash, db);
    perf_timer_stop(build_timer);
    result.build_time_ms = perf_timer_elapsed_ms(build_timer);
    perf_timer_free(build_timer);
    
    // Search phase
    PerfTimer *search_timer = perf_timer_create();
    perf_timer_start(search_timer);
    
    QueryResult *last_result = NULL;
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        if (last_result) query_result_free(last_result);
        last_result = hash_index_search(hash, db, dept_id);
    }
    
    perf_timer_stop(search_timer);
    result.search_time_ms = perf_timer_elapsed_ms(search_timer);
    result.avg_query_time_ms = result.search_time_ms / NUM_QUERIES;
    result.memory_usage = hash_index_get_memory_usage(hash);
    result.result_count = last_result ? last_result->count : 0;
    
    if (last_result) query_result_free(last_result);
    perf_timer_free(search_timer);
    hash_index_free(hash);
    
    return result;
}

// B-tree benchmark
BenchmarkResult benchmark_btree_index(Database *db) {
    BenchmarkResult result;
    strcpy(result.name, "B-tree Index");
    
    // Build phase
    BTreeIndex *btree = btree_index_create();
    PerfTimer *build_timer = perf_timer_create();
    perf_timer_start(build_timer);
    btree_index_build(btree, db);
    perf_timer_stop(build_timer);
    result.build_time_ms = perf_timer_elapsed_ms(build_timer);
    perf_timer_free(build_timer);
    
    // Search phase
    PerfTimer *search_timer = perf_timer_create();
    perf_timer_start(search_timer);
    
    QueryResult *last_result = NULL;
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        if (last_result) query_result_free(last_result);
        last_result = btree_index_search(btree, db, dept_id);
    }
    
    perf_timer_stop(search_timer);
    result.search_time_ms = perf_timer_elapsed_ms(search_timer);
    result.avg_query_time_ms = result.search_time_ms / NUM_QUERIES;
    result.memory_usage = btree_get_memory_usage(btree);
    result.result_count = last_result ? last_result->count : 0;
    
    if (last_result) query_result_free(last_result);
    perf_timer_free(search_timer);
    btree_index_free(btree);
    
    return result;
}

// Bitmap index benchmark
BenchmarkResult benchmark_bitmap_index(Database *db) {
    BenchmarkResult result;
    strcpy(result.name, "Bitmap Index");
    
    // Build phase
    BitmapIndex *bitmap = bitmap_index_create(db->num_records);
    PerfTimer *build_timer = perf_timer_create();
    perf_timer_start(build_timer);
    bitmap_index_build(bitmap, db);
    perf_timer_stop(build_timer);
    result.build_time_ms = perf_timer_elapsed_ms(build_timer);
    perf_timer_free(build_timer);
    
    // Search phase
    PerfTimer *search_timer = perf_timer_create();
    perf_timer_start(search_timer);
    
    QueryResult *last_result = NULL;
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        if (last_result) query_result_free(last_result);
        last_result = bitmap_index_search(bitmap, db, dept_id);
    }
    
    perf_timer_stop(search_timer);
    result.search_time_ms = perf_timer_elapsed_ms(search_timer);
    result.avg_query_time_ms = result.search_time_ms / NUM_QUERIES;
    result.memory_usage = bitmap_index_get_memory_usage(bitmap);
    result.result_count = last_result ? last_result->count : 0;
    
    if (last_result) query_result_free(last_result);
    perf_timer_free(search_timer);
    bitmap_index_free(bitmap);
    
    return result;
}

int main() {
    printf("\n");
    print_separator();
    printf("  DATABASE INDEX BENCHMARKING SUITE\n");
    printf("  Comparing Linear Search, Hash Index, B-tree, and Bitmap Index\n");
    print_separator();
    printf("\n");
    
    printf("Configuration:\n");
    printf("  Database Records: %d\n", NUM_RECORDS);
    printf("  Number of Queries: %d\n", NUM_QUERIES);
    printf("  Query Range: department_id 1-10\n");
    printf("  Test Iterations: %d\n\n", NUM_ITERATIONS);
    
    // Create and populate database
    printf("Creating and populating database...\n");
    Database *db = db_create();
    db_populate(db, NUM_RECORDS);
    printf("✓ Database ready with %d records\n\n", db->num_records);
    
    // Arrays to store results across iterations
    BenchmarkResult linear_results[NUM_ITERATIONS];
    BenchmarkResult hash_results[NUM_ITERATIONS];
    BenchmarkResult btree_results[NUM_ITERATIONS];
    BenchmarkResult bitmap_results[NUM_ITERATIONS];
    
    // Run benchmarks multiple times
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        printf("--- Iteration %d of %d ---\n\n", iter + 1, NUM_ITERATIONS);
        
        linear_results[iter] = benchmark_linear_search(db);
        printf("✓ Linear Search complete\n");
        
        hash_results[iter] = benchmark_hash_index(db);
        printf("✓ Hash Index complete\n");
        
        btree_results[iter] = benchmark_btree_index(db);
        printf("✓ B-tree Index complete\n");
        
        bitmap_results[iter] = benchmark_bitmap_index(db);
        printf("✓ Bitmap Index complete\n\n");
    }
    
    // Calculate averages
    BenchmarkResult linear_avg = linear_results[0];
    BenchmarkResult hash_avg = hash_results[0];
    BenchmarkResult btree_avg = btree_results[0];
    BenchmarkResult bitmap_avg = bitmap_results[0];
    
    for (int i = 1; i < NUM_ITERATIONS; i++) {
        linear_avg.search_time_ms += linear_results[i].search_time_ms;
        linear_avg.avg_query_time_ms += linear_results[i].avg_query_time_ms;
        
        hash_avg.build_time_ms += hash_results[i].build_time_ms;
        hash_avg.search_time_ms += hash_results[i].search_time_ms;
        hash_avg.avg_query_time_ms += hash_results[i].avg_query_time_ms;
        
        btree_avg.build_time_ms += btree_results[i].build_time_ms;
        btree_avg.search_time_ms += btree_results[i].search_time_ms;
        btree_avg.avg_query_time_ms += btree_results[i].avg_query_time_ms;
        
        bitmap_avg.build_time_ms += bitmap_results[i].build_time_ms;
        bitmap_avg.search_time_ms += bitmap_results[i].search_time_ms;
        bitmap_avg.avg_query_time_ms += bitmap_results[i].avg_query_time_ms;
    }
    
    linear_avg.search_time_ms /= NUM_ITERATIONS;
    linear_avg.avg_query_time_ms /= NUM_ITERATIONS;
    
    hash_avg.build_time_ms /= NUM_ITERATIONS;
    hash_avg.search_time_ms /= NUM_ITERATIONS;
    hash_avg.avg_query_time_ms /= NUM_ITERATIONS;
    
    btree_avg.build_time_ms /= NUM_ITERATIONS;
    btree_avg.search_time_ms /= NUM_ITERATIONS;
    btree_avg.avg_query_time_ms /= NUM_ITERATIONS;
    
    bitmap_avg.build_time_ms /= NUM_ITERATIONS;
    bitmap_avg.search_time_ms /= NUM_ITERATIONS;
    bitmap_avg.avg_query_time_ms /= NUM_ITERATIONS;
    
    // Print results table
    print_separator();
    printf("DETAILED BENCHMARK RESULTS (Averages across %d iterations)\n", NUM_ITERATIONS);
    print_separator();
    printf("\n%-25s | %-12s | %-12s | %-14s | %-12s\n", 
           "Index Type", "Build (ms)", "Search (ms)", "Avg Query (µs)", "Memory (KB)");
    printf("-%-25s-+-%-12s-+-%-12s-+-%-14s-+-%-12s\n",
           "------------------------", "-----------", "-----------", "-------------", "-----------");
    
    printf("%-25s | %12.4f | %12.4f | %14.4f | %12.2f\n",
           linear_avg.name, 0.0, linear_avg.search_time_ms,
           linear_avg.avg_query_time_ms * 1000, 0.0);
    
    printf("%-25s | %12.4f | %12.4f | %14.4f | %12.2f\n",
           hash_avg.name, hash_avg.build_time_ms, hash_avg.search_time_ms,
           hash_avg.avg_query_time_ms * 1000, hash_avg.memory_usage / 1024.0);
    
    printf("%-25s | %12.4f | %12.4f | %14.4f | %12.2f\n",
           btree_avg.name, btree_avg.build_time_ms, btree_avg.search_time_ms,
           btree_avg.avg_query_time_ms * 1000, btree_avg.memory_usage / 1024.0);
    
    printf("%-25s | %12.4f | %12.4f | %14.4f | %12.2f\n",
           bitmap_avg.name, bitmap_avg.build_time_ms, bitmap_avg.search_time_ms,
           bitmap_avg.avg_query_time_ms * 1000, bitmap_avg.memory_usage / 1024.0);
    
    print_separator();
    printf("\nSPEEDUP ANALYSIS (relative to Linear Search)\n");
    print_separator();
    
    double linear_total = linear_avg.search_time_ms;
    double hash_total = hash_avg.build_time_ms + hash_avg.search_time_ms;
    double btree_total = btree_avg.build_time_ms + btree_avg.search_time_ms;
    double bitmap_total = bitmap_avg.build_time_ms + bitmap_avg.search_time_ms;
    
    printf("\nSearch Phase Speedup:\n");
    printf("  Hash Index:    %.2f x faster (avg query: %.2f µs)\n",
           linear_total / hash_avg.search_time_ms,
           hash_avg.avg_query_time_ms * 1000);
    printf("  B-tree Index:  %.2f x faster (avg query: %.2f µs)\n",
           linear_total / btree_avg.search_time_ms,
           btree_avg.avg_query_time_ms * 1000);
    printf("  Bitmap Index:  %.2f x faster (avg query: %.2f µs)\n",
           linear_total / bitmap_avg.search_time_ms,
           bitmap_avg.avg_query_time_ms * 1000);
    
    printf("\nTotal Time (Build + Search):\n");
    printf("  Linear Search: %.4f ms\n", linear_total);
    printf("  Hash Index:    %.4f ms (Build: %.4f ms)\n", hash_total, hash_avg.build_time_ms);
    printf("  B-tree Index:  %.4f ms (Build: %.4f ms)\n", btree_total, btree_avg.build_time_ms);
    printf("  Bitmap Index:  %.4f ms (Build: %.4f ms)\n", bitmap_total, bitmap_avg.build_time_ms);
    
    printf("\nMemory Usage:\n");
    printf("  Hash Index:    %.2f KB\n", hash_avg.memory_usage / 1024.0);
    printf("  B-tree Index:  %.2f KB\n", btree_avg.memory_usage / 1024.0);
    printf("  Bitmap Index:  %.2f KB\n", bitmap_avg.memory_usage / 1024.0);
    
    print_separator();
    printf("RECOMMENDATIONS\n");
    print_separator();
    
    double hash_query = hash_avg.avg_query_time_ms * 1000;
    double btree_query = btree_avg.avg_query_time_ms * 1000;
    double bitmap_query = bitmap_avg.avg_query_time_ms * 1000;
    
    if (hash_query < btree_query && hash_query < bitmap_query) {
        printf("✓ RECOMMENDED: Hash Index\n");
        printf("  Best query performance for this use case\n");
    } else if (bitmap_query < hash_query && bitmap_query < btree_query) {
        printf("✓ RECOMMENDED: Bitmap Index\n");
        printf("  Best performance for low-cardinality fields\n");
    } else {
        printf("✓ RECOMMENDED: B-tree Index\n");
        printf("  Best balanced performance\n");
    }
    
    printf("\nUsage Guidance:\n");
    printf("  • Hash Index:   Best for equality searches on unique/non-unique keys\n");
    printf("  • B-tree Index: Good for range queries and sorted access\n");
    printf("  • Bitmap Index: Excellent for low-cardinality columns (few distinct values)\n");
    
    print_separator();
    printf("\n");
    
    // Cleanup
    db_free(db);
    
    return 0;
}
