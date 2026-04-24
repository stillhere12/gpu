# How It Works - Detailed Explanation

## Table of Contents
1. [Data Structures](#data-structures)
2. [Step-by-Step Execution](#step-by-step-execution)
3. [Linear Search Explained](#linear-search-explained)
4. [Indexed Search Explained](#indexed-search-explained)
5. [Indexing Mechanism](#indexing-mechanism)
6. [Code Walkthrough](#code-walkthrough)

---

## Data Structures

### Record Structure
```c
typedef struct {
    int32_t id;                      // Employee ID (unique)
    char name[MAX_NAME_LEN];         // Employee Name
    int32_t age;                     // Employee Age
    int32_t department_id;           // Department (1-10)
    float salary;                    // Employee Salary
} Record;
```

**Example Record:**
```
id = 5
name = "Employee_4"
age = 29
department_id = 5
salary = 50400.00
```

### Database Structure
```c
typedef struct {
    Record records[MAX_RECORDS];     // Array of all records (50,000 slots)
    int32_t num_records;             // How many records we actually have
} Database;
```

**Visual Representation:**
```
Database
├─ records[0]: {id: 1, name: "Employee_0", dept: 1, ...}
├─ records[1]: {id: 2, name: "Employee_1", dept: 2, ...}
├─ records[2]: {id: 3, name: "Employee_2", dept: 3, ...}
├─ ...
├─ records[49999]: {id: 50000, name: "Employee_49999", dept: 10, ...}
└─ num_records: 50000
```

### Index Structure (Hash Table)
```c
typedef struct {
    int32_t key;                    // Department ID
    int32_t *record_indices;        // Pointer to array of record positions
    int32_t count;                  // How many records in this department
    int32_t capacity;               // Space allocated
} IndexEntry;

typedef struct {
    IndexEntry *hash_table;         // Array of index entries
    int32_t size;                   // Size of hash table (10,007)
} Index;
```

**Visual Representation:**
```
Index (Hash Table)
│
├─ hash_table[0]: Empty
├─ hash_table[1]: key=1, record_indices→[0, 10, 20, 30...], count=5000
├─ hash_table[2]: key=2, record_indices→[1, 11, 21, 31...], count=5000
├─ hash_table[3]: Empty
├─ hash_table[4]: key=4, record_indices→[3, 13, 23, 33...], count=5000
├─ ...
└─ hash_table[10006]: Empty

Total: 10,007 slots (most empty, some contain department pointers)
```

---

## Step-by-Step Execution

### Phase 1: Database Creation and Population

```
Step 1: Create empty database
        Database db;
        db.num_records = 0;

Step 2: Populate with 50,000 sample records
        for (i = 0 to 49999) {
            db.records[i].id = i + 1
            db.records[i].name = "Employee_" + i
            db.records[i].department_id = (i % 10) + 1  // Cycles 1-10
            db.records[i].salary = ...
        }

Step 3: Database is ready with 50,000 records
        Each department has 5,000 employees (50,000 / 10 departments)
```

### Phase 2: Building Index

```
Step 1: Create empty hash table
        Index idx;
        idx.hash_table = allocate 10,007 slots
        idx.size = 10,007

Step 2: For each record, add to index
        for (i = 0 to 49999) {
            department_id = db.records[i].department_id
            hash_value = hash_function(department_id, 10007)
            idx.hash_table[hash_value].record_indices[...] = i
        }

Step 3: Index built! Now department lookups are fast!
```

---

## Linear Search Explained

### What Happens

```
Query: "Find all employees in Department 1"

Step 1: Start from first record (index 0)
Step 2: Check: Is department_id == 1?  → No, skip
Step 3: Check next record (index 1)
Step 4: Check: Is department_id == 1?  → No, skip
Step 5: Check next record (index 2)
        ...
        (Continues checking ALL 50,000 records)
        ...
Step 50000: Found all matches!

Time: You had to check EVERY SINGLE RECORD!
```

### Code

```c
QueryResult* query_linear_search(Database *db, int32_t field_offset, int32_t value) {
    QueryResult *result = malloc(sizeof(QueryResult));
    result->results = malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    // THIS LOOP CHECKS EVERY SINGLE RECORD!
    for (int32_t i = 0; i < db->num_records; i++) {
        if (db->records[i].department_id == value) {
            result->results[result->count++] = db->records[i];
        }
    }
    
    return result;
}
```

### Time Complexity
```
O(n) - Linear time

Explanation:
- Need to check n records (50,000)
- In worst case, need to scan entire database
- Time grows linearly with number of records
```

### Analogy
```
Finding a name in a phone book WITHOUT index:
- Start from first person
- Read name: "Is this John?"
- No, continue
- Read next name: "Is this John?"
- No, continue
- ... (check thousands of names)
```

---

## Indexed Search Explained

### What Happens

```
Query: "Find all employees in Department 1"

Step 1: Compute hash of department_id (1)
        hash_value = hash_function(1, 10007) = 7 (example)

Step 2: Look up hash_table[7]
        Finds: record_indices pointing to [0, 10, 20, 30, ..., 49990]

Step 3: Directly access these specific records!
        No need to check other 45,000 records!

Step 4: Done! Much faster!

Time: Only accessed records you needed!
```

### Code

```c
QueryResult* query_indexed_search(Index *idx, Database *db, int32_t value) {
    QueryResult *result = malloc(sizeof(QueryResult));
    result->results = malloc(sizeof(Record) * db->num_records);
    result->count = 0;
    
    // DIRECTLY ACCESS THE INDEX
    int32_t hash = hash_function(value, idx->size);
    IndexEntry *entry = &idx->hash_table[hash];
    
    // ONLY iterate through relevant records!
    for (int32_t i = 0; i < entry->count; i++) {
        int32_t record_idx = entry->record_indices[i];
        if (db->records[record_idx].department_id == value) {
            result->results[result->count++] = db->records[record_idx];
        }
    }
    
    return result;
}
```

### Time Complexity
```
O(k) where k = number of matching records

Explanation:
- Hash lookup: O(1) - instant
- Iterate only matching records: O(k)
- For Department 1: k = 5,000 (not 50,000!)
- Much faster than checking all 50,000!
```

### Analogy
```
Finding a name in a phone book WITH index:
- Look up "J" in index
- Index says: "J names are on pages 200-210"
- Go directly to page 200
- Find John immediately!
- Don't need to read pages 1-199!
```

---

## Indexing Mechanism

### Hash Function

```c
static int32_t hash_function(int32_t key, int32_t size) {
    return ((key * 2654435761U) ^ (key >> 16)) % size;
}
```

**What it does:**
- Takes a key (department_id)
- Multiplies by a large prime number
- XOR with shifted bits
- Mod by table size
- Returns index 0 to 10,006

**Example:**
```
hash_function(1, 10007)
= ((1 * 2654435761) ^ (1 >> 16)) % 10007
= ((2654435761) ^ (0)) % 10007
= 2654435761 % 10007
= 7

hash_function(2, 10007) = 9
hash_function(3, 10007) = 11
hash_function(4, 10007) = 13
...
```

### Building the Index

```c
void index_build(Index *idx, Database *db, int32_t field_offset) {
    for (int32_t i = 0; i < db->num_records; i++) {
        int32_t value = db->records[i].department_id;
        int32_t hash = hash_function(value, idx->size);
        
        // Get the entry at this hash position
        IndexEntry *entry = &idx->hash_table[hash];
        
        // Expand if needed
        if (entry->count >= entry->capacity) {
            entry->capacity *= 2;
            entry->record_indices = realloc(...);
        }
        
        // Add record index to this department's list
        entry->record_indices[entry->count++] = i;
        entry->key = value;
    }
}
```

**Step by Step:**
```
Record 0: dept_id=1, hash=7
  → Add 0 to hash_table[7].record_indices[]
  → hash_table[7].count = 1

Record 10: dept_id=1, hash=7
  → Add 10 to hash_table[7].record_indices[]
  → hash_table[7].count = 2

Record 20: dept_id=1, hash=7
  → Add 20 to hash_table[7].record_indices[]
  → hash_table[7].count = 3

... (continue for all 50,000 records)

Result:
hash_table[7].record_indices = [0, 10, 20, 30, ..., 49990]
hash_table[7].count = 5000
```

---

## Code Walkthrough

### main.c - The Main Flow

```c
int main() {
    // 1. CREATE DATABASE
    Database *db = db_create();
    db_populate(db, NUM_RECORDS);
    // Now db has 50,000 records
    
    // 2. BUILD INDEX
    Index *idx = index_create(HASH_TABLE_SIZE);
    PerfTimer *index_timer = perf_timer_create();
    perf_timer_start(index_timer);
    index_build(idx, db, 0);  // Takes 1.3 ms
    perf_timer_stop(index_timer);
    
    // 3. BENCHMARK LINEAR SEARCH
    PerfTimer *linear_timer = perf_timer_create();
    perf_timer_start(linear_timer);
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        QueryResult *result = query_linear_search(db, 0, dept_id);
        query_result_free(result);
    }
    perf_timer_stop(linear_timer);
    // linear_timer->elapsed_ms ≈ 218 ms
    
    // 4. BENCHMARK INDEXED SEARCH
    PerfTimer *indexed_timer = perf_timer_create();
    perf_timer_start(indexed_timer);
    for (int i = 0; i < NUM_QUERIES; i++) {
        int dept_id = (i % 10) + 1;
        QueryResult *result = query_indexed_search(idx, db, dept_id);
        query_result_free(result);
    }
    perf_timer_stop(indexed_timer);
    // indexed_timer->elapsed_ms ≈ 87 ms
    
    // 5. PRINT RESULTS
    // Shows: 2.51x speedup, 60% improvement
    
    // 6. CLEANUP
    index_free(idx);
    db_free(db);
}
```

---

## Memory Usage

### Linear Search
```
Database:        50,000 records × 88 bytes = ~4.4 MB
Index:           None needed
Total:           ~4.4 MB
```

### Indexed Search
```
Database:        50,000 records × 88 bytes = ~4.4 MB
Index:           10,007 entries + 50,000 pointers = ~0.4 MB
Total:           ~4.8 MB

Trade-off: +0.4 MB memory for 2.51x speed!
```

---

## Summary Table

| Aspect | Linear Search | Indexed Search |
|--------|---------------|----------------|
| Method | Check every record | Use hash table lookup |
| Time | O(n) = 50,000 checks | O(k) = 5,000 checks |
| Speed | 218 ms | 87 ms |
| Memory | ~4.4 MB | ~4.8 MB |
| Speedup | 1x (baseline) | 2.51x |
| When to use | Small datasets | Large datasets |

---

## Key Takeaways

1. **Database** = Collection of structured records
2. **Query** = Request to find matching records
3. **Linear Search** = Check every record (slow!)
4. **Index** = Pre-organized lookup table
5. **Indexed Search** = Use index to find quickly
6. **Trade-off** = Use more memory to save time
7. **Real-world** = Used in ALL databases (MySQL, PostgreSQL, etc.)

---

## Next: Read FLOWCHARTS.md for visual diagrams!
