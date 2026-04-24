#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>

typedef struct {
    double start_time;
    double end_time;
    double elapsed_ms;
} PerfTimer;

// Function declarations
PerfTimer* perf_timer_create();
void perf_timer_start(PerfTimer *t);
void perf_timer_stop(PerfTimer *t);
double perf_timer_elapsed_ms(PerfTimer *t);
void perf_timer_free(PerfTimer *t);

#endif // BENCHMARK_H
