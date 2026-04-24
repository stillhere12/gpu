# Flowcharts and Visual Diagrams

## 1. Overall Program Flow

```
┌─────────────────────────────────────┐
│   START PROGRAM                     │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   CREATE DATABASE                   │
│   - Allocate 50,000 record slots    │
│   - Set num_records = 0             │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   POPULATE DATABASE                 │
│   - Fill 50,000 sample records      │
│   - Department IDs: 1-10 (cycling)  │
│   - Set num_records = 50,000        │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   CREATE INDEX                      │
│   - Create hash table (10,007 slots)│
│   - Initialize all entries empty    │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   BUILD INDEX                       │
│   - For each record:                │
│     • Hash department_id            │
│     • Store record position in index│
│   - Time: 1.3 ms                    │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   START BENCHMARKS                  │
└──────────────┬──────────────────────┘
               │
        ┌──────┴──────┐
        │             │
        ▼             ▼
    ┌─────────┐  ┌──────────────┐
    │ LINEAR  │  │   INDEXED    │
    │ SEARCH  │  │   SEARCH     │
    │ 1000x   │  │   1000x      │
    │         │  │              │
    │218.09ms │  │  86.77ms     │
    └────┬────┘  └────┬─────────┘
         │             │
         └──────┬──────┘
                ▼
    ┌─────────────────────────────────┐
    │   COMPARE RESULTS               │
    │   - Speedup: 2.51x              │
    │   - Improvement: 60.21%         │
    └────────────┬────────────────────┘
                 │
                 ▼
    ┌─────────────────────────────────┐
    │   PRINT RESULTS & CLEANUP       │
    │   - Free database memory        │
    │   - Free index memory           │
    │   - End program                 │
    └─────────────────────────────────┘
```

---

## 2. Linear Search Flowchart

```
┌──────────────────────────────────────┐
│   LINEAR SEARCH QUERY                │
│   Find: department_id == X           │
└──────────────┬───────────────────────┘
               │
               ▼
    ┌──────────────────────────┐
    │ results = empty list     │
    │ i = 0                    │
    └──────────────┬───────────┘
                   │
                   ▼
    ┌──────────────────────────┐
    │ Is i < 50,000?           │
    └──────┬──────────────┬────┘
           │ YES          │ NO
           ▼              │
    ┌──────────────────────┐ │
    │ Check record[i]      │ │
    │ department_id == X?  │ │
    └──────┬────────┬──────┘ │
           │YES     │NO      │
           ▼        ▼        │
    ┌─────────┐ ┌────────┐   │
    │ ADD to  │ │ SKIP   │   │
    │ results │ │        │   │
    └────┬────┘ └────┬───┘   │
         │           │       │
         └─────┬─────┘       │
               │             │
               ▼             │
    ┌──────────────────────┐ │
    │ i = i + 1            │ │
    └──────────────┬───────┘ │
                   │         │
        ┌──────────┴─────────┘
        │ (loop back to check)
        ▼
    ┌──────────────────────────────┐
    │ RETURN results               │
    │ (All matching records found) │
    └──────────────────────────────┘

⏱️  Time: Must check ALL 50,000 records!
📊 Complexity: O(n)
```

---

## 3. Indexed Search Flowchart

```
┌──────────────────────────────────────┐
│   INDEXED SEARCH QUERY               │
│   Find: department_id == X           │
└──────────────┬───────────────────────┘
               │
               ▼
    ┌──────────────────────────┐
    │ results = empty list     │
    │ hash_value =             │
    │   hash(X, 10007)         │
    │                          │
    │ Example: X=1 → hash=7    │
    └──────────────┬───────────┘
                   │
                   ▼
    ┌──────────────────────────────┐
    │ Look up index[hash_value]    │
    │ → Get record_indices[]       │
    │ → Get count (how many match) │
    │                              │
    │ Example:                     │
    │ index[7].record_indices =    │
    │   [0, 10, 20, ..., 49990]    │
    │ index[7].count = 5000        │
    └──────────────┬───────────────┘
                   │
                   ▼
    ┌──────────────────────────┐
    │ i = 0                    │
    └──────────────┬───────────┘
                   │
                   ▼
    ┌──────────────────────────┐
    │ Is i < count (5000)?     │
    │ (NOT 50,000!)            │
    └──────┬──────────────┬────┘
           │ YES          │ NO
           ▼              │
    ┌──────────────────────┐ │
    │ record_idx =         │ │
    │ record_indices[i]    │ │
    │ (Get position from   │ │
    │  index, e.g., 0)     │ │
    └──────────┬───────────┘ │
               │             │
               ▼             │
    ┌──────────────────────┐ │
    │ Verify:              │ │
    │ record[record_idx]   │ │
    │ .department_id == X? │ │
    │ (Usually YES!)       │ │
    └──────┬───────────────┘ │
           │                 │
           ▼                 │
    ┌──────────────────────┐ │
    │ ADD to results       │ │
    └────┬─────────────────┘ │
         │                   │
         ▼                   │
    ┌──────────────────────┐ │
    │ i = i + 1            │ │
    └──────────┬───────────┘ │
               │             │
        ┌──────┴──────────────┘
        │ (loop back, but only 5000 times!)
        ▼
    ┌──────────────────────────────┐
    │ RETURN results               │
    │ (All matching records found) │
    └──────────────────────────────┘

⏱️  Time: Check only 5,000 records (not 50,000)!
📊 Complexity: O(k) where k = matching records
✨ Speedup: 50,000 / 5,000 = 10x fewer checks!
```

---

## 4. Hash Index Structure

```
Database (50,000 records)
├─ Record 0: dept_id=1
├─ Record 1: dept_id=2
├─ Record 2: dept_id=3
├─ ...
└─ Record 49999: dept_id=10

                    │
                    │ INDEX BUILT
                    │ (Hash all records)
                    ▼

Hash Table (10,007 slots)

hash[0]  ┌─ Empty
hash[1]  ├─ Empty
hash[2]  ├─ Empty
hash[3]  ├─ Empty
hash[4]  ├─ Empty
hash[5]  ├─ Empty
hash[6]  ├─ Empty
hash[7]  ├─ ✓ dept_id=1
         │  └─ record_indices: [0, 10, 20, ..., 49990]
         │     count: 5000
hash[8]  ├─ Empty
hash[9]  ├─ ✓ dept_id=2
         │  └─ record_indices: [1, 11, 21, ..., 49991]
         │     count: 5000
hash[10] ├─ Empty
...
hash[10006] ├─ Empty

QUERY: Find department_id = 1
1. hash(1) = 7
2. index[7].record_indices = [0, 10, 20, ..., 49990]
3. Return 5000 records directly!
4. No need to check other 45,000 records!
```

---

## 5. Performance Comparison

```
LINEAR SEARCH (50,000 iterations)
═════════════════════════════════════

Iteration 1:   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
Iteration 2:   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
Iteration 3:   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
... (50,000 iterations!)
Iteration 50000: ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

Total: █████████████████████ 218.09 ms

INDEXED SEARCH (5,000 iterations)
═════════════════════════════════

Hash lookup: ▓
Iteration 1: ░░░░░░░░░░░░
Iteration 2: ░░░░░░░░░░░░
... (5,000 iterations, NOT 50,000!)
Iteration 5000: ░░░░░░░░░░░░

Total: ████████ 86.77 ms

SPEEDUP: █████████ 2.51x FASTER!
```

---

## 6. Memory Layout

```
BEFORE: Database with NO Index
════════════════════════════════

Database Structure (4.4 MB)
┌─────────────────────────────────────┐
│  Record[0]    : {1, Emp_0, 1, ...}  │
│  Record[1]    : {2, Emp_1, 2, ...}  │
│  Record[2]    : {3, Emp_2, 3, ...}  │
│  ...                                │
│  Record[49999]: {50000, Emp_49999...}│
└─────────────────────────────────────┘
    Total: ~4.4 MB



AFTER: Database WITH Index
════════════════════════════════

Database Structure (4.4 MB)
┌─────────────────────────────────────┐
│  Record[0]    : {1, Emp_0, 1, ...}  │
│  Record[1]    : {2, Emp_1, 2, ...}  │
│  Record[2]    : {3, Emp_2, 3, ...}  │
│  ...                                │
│  Record[49999]: {50000, Emp_49999...}│
└─────────────────────────────────────┘

Index Structure (0.4 MB)
┌─────────────────────────────────────┐
│  hash[0]      : NULL                │
│  hash[1]      : NULL                │
│  ...                                │
│  hash[7]      : →[0,10,20,...,5000] │  ← Points to dept 1 records
│  hash[9]      : →[1,11,21,...,5000] │  ← Points to dept 2 records
│  ...                                │
└─────────────────────────────────────┘

Total: ~4.8 MB (+0.4 MB for index)

Trade-off: Pay 0.4 MB to get 2.51x speed!
```

---

## 7. Index Building Process

```
BUILDING INDEX: Add records one by one
═════════════════════════════════════════

Database Records:
[0] dept=1  ─┐
[1] dept=2  ─┼─ Dept cycles 1-10
[2] dept=3  ─│
...         │
[9] dept=10─┤
[10] dept=1 ─┤
...         │
[49999] dept=10 ─┘

Step 1: For Record 0 (dept=1)
   hash(1) = 7
   index[7].record_indices = [0]
   index[7].count = 1

Step 2: For Record 1 (dept=2)
   hash(2) = 9
   index[9].record_indices = [1]
   index[9].count = 1

Step 3: For Record 10 (dept=1)
   hash(1) = 7
   index[7].record_indices = [0, 10]
   index[7].count = 2

... (continue for all 50,000)

Final Result:
   index[7].record_indices = [0, 10, 20, ..., 49990]  (5,000 records)
   index[7].count = 5000

   index[9].record_indices = [1, 11, 21, ..., 49991]  (5,000 records)
   index[9].count = 5000
   ... and so on for all 10 departments
```

---

## 8. Query Execution Timeline (1000 queries)

```
LINEAR SEARCH
═════════════

Query 1:  ████████████████████ (0.218 ms)
Query 2:  ████████████████████ (0.218 ms)
Query 3:  ████████████████████ (0.218 ms)
...
Query 1000: ████████████████████ (0.218 ms)

Total: 218.09 ms
─────────────────────────────────────────

INDEXED SEARCH
══════════════

Query 1:  ████████ (0.087 ms)
Query 2:  ████████ (0.087 ms)
Query 3:  ████████ (0.087 ms)
...
Query 1000: ████████ (0.087 ms)

Total: 86.77 ms
─────────────────────────────────────────

TIME SAVED: 218.09 - 86.77 = 131.32 ms!
Percentage: (131.32 / 218.09) × 100 = 60.21%
```

---

## 9. Hash Function Behavior

```
Input: Department ID (1-10)
Output: Hash table index (0-10006)

dept_id → hash_function(dept_id, 10007)

1 → 7      (hash table[7])
2 → 9      (hash table[9])
3 → 11     (hash table[11])
4 → 13     (hash table[13])
5 → 15     (hash table[15])
6 → 17     (hash table[17])
7 → 19     (hash table[19])
8 → 21     (hash table[21])
9 → 23     (hash table[23])
10 → 25    (hash table[25])

Distribution: Spreads across hash table evenly
Result: No collisions in this case!
```

---

## 10. Complete Query Execution Comparison

```
FINDING ALL EMPLOYEES IN DEPARTMENT 1
═══════════════════════════════════════

LINEAR SEARCH
─────────────

START
  │
  ├─ Check record[0]: dept=1? YES ✓ Add to results
  ├─ Check record[1]: dept=2? NO
  ├─ Check record[2]: dept=3? NO
  ├─ Check record[3]: dept=4? NO
  ├─ ...
  ├─ Check record[9]: dept=10? NO
  ├─ Check record[10]: dept=1? YES ✓ Add to results
  ├─ Check record[11]: dept=2? NO
  ├─ ...
  └─ Check record[49999]: dept=10? NO

RESULT: Found 5,000 employees ✓
TIME: Checked 50,000 records ⏱ 0.218 ms

                            ↓ vs ↓

INDEXED SEARCH
──────────────

START
  │
  ├─ Compute hash(1) = 7
  ├─ Look up index[7]
  ├─ Found: record_indices = [0, 10, 20, ..., 49990]
  │
  ├─ Check record[0]: dept=1? YES ✓ Add to results
  ├─ Check record[10]: dept=1? YES ✓ Add to results
  ├─ Check record[20]: dept=1? YES ✓ Add to results
  ├─ ...
  └─ Check record[49990]: dept=1? YES ✓ Add to results

RESULT: Found 5,000 employees ✓
TIME: Checked 5,000 records only! ⏱ 0.087 ms

SPEEDUP: 2.51x faster! 🚀
```

---

## Key Takeaways from Flowcharts

1. **Linear Search** → Check everything (slow)
2. **Index Lookup** → Go directly to target (fast)
3. **Hash Function** → Instantly finds hash bucket
4. **Memory Trade-off** → Use 0.4 MB extra to save time
5. **Scalability** → Index helps even more with larger datasets
6. **Speedup** → 2.51x for this dataset, could be 10x+ for larger ones

---

## Next: Read TEACHER_Q&A.md for interview prep!
