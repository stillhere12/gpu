# Index Implementation & Benchmarking Guide

## Overview
This project implements and benchmarks three different index structures for database query optimization:
- **Linear Search** (Baseline - no index)
- **Hash Index** (Optimized hash table with chaining)
- **B-tree Index** (Balanced tree structure)
- **Bitmap Index** (Bit vector for low-cardinality fields)

## Architecture

### File Structure
```
├── database.h/c          # Core database and original hash index
├── btree_index.h/c       # B-tree implementation
├── hash_index.h/c        # Enhanced hash index with chaining
├── bitmap_index.h/c      # Bitmap index for low-cardinality fields
├── index_interface.h     # Unified interface for all indexes
├── benchmark.h/c         # Performance measurement utilities
├── benchmark_all.c       # Comprehensive benchmark suite
├── main.c                # Original single-threaded benchmark
└── Makefile              # Build configuration
```

## Index Implementations

### 1. Linear Search (Baseline)
- **Time Complexity**: O(n) per query
- **Space Complexity**: O(1) - no index structure
- **Use Case**: Small datasets or when order matters
- **Characteristics**:
  - No index building overhead
  - Cache-friendly for sequential access
  - Good for range queries

### 2. Hash Index
- **Time Complexity**: O(1) average, O(n) worst case
- **Space Complexity**: O(n)
- **Features**:
  - Collision resolution via chaining
  - Dynamic bucket growth
  - Separate chains for each hash bucket
  - Perfect for equality searches on unique/non-unique keys

**Implementation Details:**
```c
typedef struct HashIndexEntry {
    int32_t key;
    int32_t *record_indices;      // Records with this key
    int32_t count;
    int32_t capacity;
    struct HashIndexEntry *next;  // Chaining for collisions
} HashIndexEntry;
```

### 3. B-tree Index
- **Time Complexity**: O(log n) per query
- **Space Complexity**: O(n)
- **Features**:
  - Balanced tree with order 16
  - Support for duplicate keys
  - Good for range queries and sorted traversal
  - Self-balancing (simplified for this implementation)

**B-tree Properties:**
- Order: 16 (branching factor)
- Max keys per node: 31
- All leaves at same level
- Maintains sorted order

### 4. Bitmap Index
- **Time Complexity**: O(n) scan (but very fast in practice)
- **Space Complexity**: O(d*n/8) where d = distinct values
- **Features**:
  - One bitmap per distinct value
  - Excellent for low-cardinality fields
  - Very memory-efficient
  - Perfect for department_id (10 distinct values)

**Memory Efficiency:**
```
For 100,000 records with 10 distinct values:
Bitmap size = 10 × (100,000 / 8) = 125 KB
Hash Index ≈ 478 KB
B-tree ≈ 2400 KB
```

## Building the Project

### Prerequisites
- GCC compiler
- Standard C library
- No external dependencies

### Build Commands

```bash
# Build all targets
make all

# Build and run original benchmark
make run

# Build and run comprehensive benchmark
make run-benchmark

# Build and run both
make run-all

# Clean build artifacts
make clean
```

### Compiler Flags
```
-O3              # Aggressive optimization
-march=native    # CPU-specific optimizations
-ffast-math      # Relaxed floating-point arithmetic
-Wall -Wextra    # Enable all warnings
```

## Benchmark Results

### Configuration
- **Database Size**: 100,000 records
- **Queries**: 10,000 equality searches
- **Query Range**: department_id (1-10)
- **Iterations**: 3 (averaged)

### Results Summary

| Index Type | Build (ms) | Search (ms) | Avg Query (µs) | Memory (KB) | Speedup |
|---|---|---|---|---|---|
| Linear Search | 0.00 | 6297.07 | 629.71 | 0 | 1.0x |
| Hash Index | 0.72 | 1002.44 | 100.24 | 478.51 | 6.28x |
| B-tree Index | 0.66 | 2.81 | 0.28 | 2400.02 | 2241.75x |
| Bitmap Index | 0.53 | 2332.05 | 233.21 | 124.44 | 2.70x |

### Performance Analysis

#### B-tree Index (RECOMMENDED)
- **Advantages**:
  - Exceptional query performance (2241x faster)
  - Fast index building
  - Good for range queries
  - Balanced tree structure
  
- **Disadvantages**:
  - Higher memory usage (2.4 MB)
  - Slower than hash for equality searches alone
  - More complex implementation

#### Hash Index
- **Advantages**:
  - Good query performance (6.28x faster)
  - Lower memory than B-tree
  - Simple implementation
  - Fast lookup for non-duplicate keys
  
- **Disadvantages**:
  - Slower than B-tree
  - Poor range query performance
  - Collision resolution overhead
  - Hash collisions on large datasets

#### Bitmap Index
- **Advantages**:
  - Smallest memory footprint (124 KB)
  - Excellent for low-cardinality fields
  - Fast index building
  - Good for bit-level operations
  
- **Disadvantages**:
  - Limited to equality searches
  - Memory overhead with many distinct values
  - Slower than hash for this use case
  - Sequential scan required

## Usage Examples

### Running the Benchmark

```bash
# Original benchmark (simpler output)
./db_optimizer

# Comprehensive benchmark (all three indexes)
./benchmark_all
```

### Sample Output
```
================================================================================
  DATABASE INDEX BENCHMARKING SUITE
  Comparing Linear Search, Hash Index, B-tree, and Bitmap Index
================================================================================

Index Type                | Build (ms)   | Search (ms)  | Avg Query (µs) | Memory (KB)
Linear Search (Baseline)  |       0.0000 |    6297.0660 |       629.7066 |         0.00
Hash Index                |       0.7194 |    1002.4426 |       100.2443 |       478.51
B-tree Index              |       0.6580 |       2.8090 |         0.2809 |      2400.02
Bitmap Index              |       0.5349 |    2332.0527 |       233.2053 |       124.44
```

## Optimization Techniques Used

### 1. Memory Optimization
- Pre-allocated arrays to reduce fragmentation
- Dynamic resizing with doubling strategy
- Bitmap uses bit-level storage

### 2. CPU Optimization
- Cache-friendly sequential access
- O3 compiler optimization
- SIMD-friendly data structures
- native CPU instruction set

### 3. Algorithm Optimization
- O(1) hash lookups
- O(log n) B-tree traversal
- Fast bitmap scanning

## Recommendations by Use Case

### Choosing an Index

| Scenario | Best Index | Reason |
|----------|-----------|--------|
| Equality searches, many queries | B-tree | Best overall performance |
| Low-cardinality field (<100 values) | Bitmap | Memory efficient |
| High-cardinality unique keys | Hash | Fast equality lookup |
| Range queries | B-tree | Can traverse sorted keys |
| Memory-constrained | Bitmap | Smallest footprint |
| Large dataset (>1M rows) | B-tree | Scales well |

## Extension Points

### To Add New Index Type
1. Create `new_index.h` with structure definitions
2. Create `new_index.c` with implementation
3. Implement: `create()`, `build()`, `search()`, `free()`, `get_memory_usage()`
4. Add to `benchmark_all.c` with new benchmark function
5. Update Makefile

### To Add Range Query Support
1. Modify search functions to accept range parameters
2. For B-tree: traverse from min to max key
3. For Bitmap: OR multiple bitmaps
4. For Hash: requires additional data structure

## Performance Tuning Parameters

### B-tree
```c
#define BTREE_ORDER 16  // Increase for wider nodes
```

### Hash Index
```c
#define HASH_INDEX_SIZE 10007  // Tune for collision rate
```

### Bitmap
```c
// Automatically sized based on record count
bitmap->bitmap_size = (num_records + 7) / 8;
```

## Compilation Details

### Original Database Code
- Simpler hash table implementation
- No duplicate key handling
- Good baseline for comparison

### New Index Implementations
- Enhanced with duplicate key support
- Error checking
- Memory accounting
- Proper cleanup

## Testing

### Unit Tests
```bash
gcc -O2 -Wall -Wextra -c test_indexes.c -o test_indexes.o
gcc -O2 -o test_indexes test_indexes.o *.o -lm
./test_indexes
```

### Validation Checks
- Record count matches expected results
- No memory leaks on cleanup
- Correct sorting in B-tree
- Bitmap bit accuracy

## Common Issues & Solutions

### B-tree Shows Wrong Results
- Check duplicate key handling in insertion
- Verify record counting in searches
- Ensure leaf node is properly marked

### Hash Collisions
- Adjust HASH_INDEX_SIZE to prime numbers
- Use different hash functions
- Increase initial bucket count

### Bitmap Memory Issues
- Verify bitmap_size calculation
- Check for buffer overflows
- Monitor for sparse data

## Future Improvements

1. **Adaptive Indexing**: Switch index types based on query patterns
2. **Composite Indexes**: Index multiple columns
3. **Partial Indexes**: Index subset of data
4. **Index Compression**: Reduce memory usage
5. **Multi-threaded Builds**: Parallel index construction
6. **Query Optimization**: Cost-based index selection

## References

- B-tree implementation: Balanced search tree concepts
- Hash table: Collision resolution via chaining
- Bitmap index: Bit-vector compression techniques
- Performance measurement: High-resolution timers

## License

Educational project for database optimization learning.

---

**Project Location**: `/home/balaji/gpuproject/hpc_db_optimization/`
**Last Updated**: 2026-04-25
