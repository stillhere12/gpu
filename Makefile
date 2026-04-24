CC = gcc
CFLAGS = -O3 -Wall -Wextra -march=native -ffast-math
LDFLAGS = -lm

TARGET = db_optimizer
SOURCES = main.c database.c benchmark.c
HEADERS = database.h benchmark.h
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all run clean
