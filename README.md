# HPC Database Query Optimization Mini Project

## Quick Overview

This project demonstrates **database query optimization** - a fundamental concept in **High-Performance Computing (HPC)**.

**What does it do?**
- Creates a database with 50,000 employee records
- Searches the database in TWO ways:
  1. **Slow way** (Linear Search) - checks every single record ❌
  2. **Fast way** (Indexed Search) - uses an index like a phone book ✓
- Compares performance: **2.51x faster** with optimization!

---

## Key Concepts Explained

### 1. **Database**
A collection of data organized in records. Each record contains:
```
Record = {ID, Name, Age, Department, Salary}
```
We have 50,000 such records in memory.

### 2. **Query**
A request to find records matching certain criteria. Example:
```
"Find all employees in Department 1"
```

### 3. **Linear Search (Unoptimized)**
```
Go through EVERY record one by one
If record matches → add to results
Repeat until all records checked
```
⏱️ **Time: 218 ms for 1,000 queries**

### 4. **Indexed Search (Optimized)**
```
Use a special lookup table (INDEX) like dictionary
Index maps: Department ID → List of employees
Directly access employees in that department
NO need to check all records!
```
⏱️ **Time: 87 ms for 1,000 queries**

---

## File Structure

```
hpc_db_optimization/
├── database.h          → Database structure definitions
├── database.c          → Database implementation
├── benchmark.h         → Timer utilities
├── benchmark.c         → Timer implementation
├── main.c              → Main program with benchmarks
├── Makefile            → Build instructions
├── README.md           → This file
├── HOW_IT_WORKS.md     → Detailed explanations
├── FLOWCHARTS.md       → Visual flowcharts
├── TEACHER_Q&A.md      → Common questions answered
└── OPTIMIZATION_EXPLAINED.md → Deep dive into optimization
```

---

## Quick Start

### Compile
```bash
cd hpc_db_optimization
make
```

### Run
```bash
./db_optimizer
```

### Clean
```bash
make clean
```

---

## Output Explanation

```
=== HPC Database Query Optimization Mini Project ===

Creating database with 50000 records...
✓ Database created successfully

Building index on department_id...
✓ Index built in 1.3291 ms

=== Linear Search Benchmark ===
Running 1000 queries with linear search...
Total time: 218.0930 ms
Avg query time: 0.218093 ms

=== Indexed Search Benchmark ===
Running 1000 queries with indexed search...
Total time: 86.7729 ms
Avg query time: 0.086773 ms

=== Performance Comparison ===
Speedup: 2.51 x
Improvement: 60.21 %
```

**What does this mean?**
- Linear search took 218 ms (slow)
- Indexed search took 87 ms (fast)
- Indexed is **2.51 times faster**
- We saved **60.21% of time**

---

## Real-World Applications

| Problem | Solution |
|---------|----------|
| Finding a customer by ID in millions of records | Use Index on customer_id |
| Searching phone numbers by country | Use Index on country_code |
| Finding posts by hashtag | Use Index on hashtag |
| Database query optimization | Use Index on frequently queried fields |

---

## Learning Outcomes

After this project, you understand:
- ✓ How databases store and retrieve data
- ✓ Why some queries are fast and others slow
- ✓ How indexing improves performance
- ✓ How to measure and benchmark code
- ✓ Trade-offs: Memory (index) vs Speed (faster queries)

---

## Files to Read Next

1. **FLOWCHARTS.md** - See visual flowcharts
2. **HOW_IT_WORKS.md** - Deep dive into code
3. **TEACHER_Q&A.md** - Prepare for questions
4. **OPTIMIZATION_EXPLAINED.md** - Understand optimization

---

## Simple Analogy

### Without Index (Linear Search)
```
You: "Find John Smith's number"
Directory: "Let me check every person one by one..."
(Checks 50,000 people!)
```

### With Index (Indexed Search)
```
You: "Find John Smith's number"
Directory: "Let me look up 'S' in my index..."
Index shows: "S section is on page 500"
Goes directly to page 500, finds John!
```

---

## Code Files Overview

### database.h (Header)
- Defines data structures: `Record`, `Database`, `Index`
- Function declarations for database operations

### database.c (Implementation)
- `db_create()` - Create empty database
- `db_populate()` - Fill with sample data
- `index_build()` - Create lookup index
- `query_linear_search()` - Slow search
- `query_indexed_search()` - Fast search

### benchmark.c
- `perf_timer_start()` - Start measuring
- `perf_timer_stop()` - Stop measuring
- `perf_timer_elapsed_ms()` - Report elapsed time

### main.c
- Sets up database and index
- Runs performance benchmarks
- Prints comparison results

---

## Performance Metrics Explained

```
Speedup = Linear_Time / Indexed_Time = 218 / 87 = 2.51x
Improvement = (Linear - Indexed) / Linear * 100 = 60.21%
```

This means indexed search is **60% faster** than linear search.

---

## Next Steps

- Modify the code to index on different fields
- Try with more records (100,000, 1,000,000)
- Implement different indexing strategies (B-tree, etc.)
- Add persistence to save/load database from disk

---

## Questions?

Read **TEACHER_Q&A.md** for common questions and detailed answers!
