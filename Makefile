CC = gcc
CFLAGS = -O3 -Wall -Wextra -march=native -ffast-math
LDFLAGS = -lm

# Original target
TARGET = db_optimizer
SOURCES = main.c database.c benchmark.c
HEADERS = database.h benchmark.h
OBJECTS = $(SOURCES:.c=.o)

# New targets for different index implementations
TARGET_BENCHMARK_ALL = benchmark_all
BENCHMARK_SOURCES = benchmark_all.c database.c benchmark.c btree_index.c hash_index.c bitmap_index.c
BENCHMARK_HEADERS = database.h benchmark.h index_interface.h btree_index.h hash_index.h bitmap_index.h
BENCHMARK_OBJECTS = $(BENCHMARK_SOURCES:.c=.o)

all: $(TARGET) $(TARGET_BENCHMARK_ALL)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

$(TARGET_BENCHMARK_ALL): $(BENCHMARK_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET_BENCHMARK_ALL) $(BENCHMARK_OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

run-benchmark: $(TARGET_BENCHMARK_ALL)
	./$(TARGET_BENCHMARK_ALL)

run-all: run run-benchmark

clean:
	rm -f $(OBJECTS) $(BENCHMARK_OBJECTS) $(TARGET) $(TARGET_BENCHMARK_ALL)

.PHONY: all run run-benchmark run-all clean
