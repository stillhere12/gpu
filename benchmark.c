#include <stdlib.h>
#include <sys/time.h>
#include "benchmark.h"

// Get current time in milliseconds
static double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0;
}

// Create timer
PerfTimer* perf_timer_create() {
    PerfTimer *t = (PerfTimer *)malloc(sizeof(PerfTimer));
    t->start_time = 0.0;
    t->end_time = 0.0;
    t->elapsed_ms = 0.0;
    return t;
}

// Start timer
void perf_timer_start(PerfTimer *t) {
    t->start_time = get_time_ms();
}

// Stop timer
void perf_timer_stop(PerfTimer *t) {
    t->end_time = get_time_ms();
    t->elapsed_ms = t->end_time - t->start_time;
}

// Get elapsed time
double perf_timer_elapsed_ms(PerfTimer *t) {
    return t->elapsed_ms;
}

// Free timer
void perf_timer_free(PerfTimer *t) {
    if (t) {
        free(t);
    }
}
