

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/resource.h>  // For user and system CPU time
#include <sys/time.h>      // For memory usage

#include "utils.c"

volatile sig_atomic_t signal_received = 0;
struct timespec start, end, sleep_time;

// Define the CSV file path
const char* csv_filename = "benchmark_results.csv";

// Function to get system metrics
void get_system_metrics(long *user_cpu_time, long *system_cpu_time, long *memory_usage) {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        *user_cpu_time = usage.ru_utime.tv_sec * 1000000 + usage.ru_utime.tv_usec;
        *system_cpu_time = usage.ru_stime.tv_sec * 1000000 + usage.ru_stime.tv_usec;
        *memory_usage = usage.ru_maxrss;  // Maximum resident set size in kilobytes
    } else {
        perror("getrusage");
        exit(EXIT_FAILURE);
    }
}

// Function to write data to CSV
void save_benchmark_data_to_csv_file(const char *filename, const char *method, long iteration, long long latency_ns, long long jitter_ns, long user_cpu_time, long system_cpu_time, long memory_usage) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

        fprintf(file, "%s,%ld,%lld,%lld,%ld,%ld,%ld\n",
            method, iteration, latency_ns, jitter_ns, user_cpu_time, system_cpu_time, memory_usage);


    fclose(file);
}

// Signal handler for signal latency benchmark
void signal_handler(int signum) {
    signal_received = 1;
    clock_gettime(CLOCK_MONOTONIC, &end);
}

// Benchmark function for nanosleep
void benchmark_nanosleep() {
    long long total_jitter = 0;
    long long max_jitter = 0;
    long long min_jitter = LLONG_MAX;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000000; // 1 ms

    for (int i = 0; i < ITERATIONS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        nanosleep(&sleep_time, NULL);
        clock_gettime(CLOCK_MONOTONIC, &end);

        long long elapsed = (end.tv_sec - start.tv_sec) * NS_PER_SEC + (end.tv_nsec - start.tv_nsec);
        long long jitter = elapsed - sleep_time.tv_nsec;

        total_jitter += llabs(jitter);
        if (jitter > max_jitter) max_jitter = jitter;
        if (jitter < min_jitter) min_jitter = jitter;

        // Get system metrics
        long user_cpu_time, system_cpu_time, memory_usage;
        get_system_metrics(&user_cpu_time, &system_cpu_time, &memory_usage);

        // Save data to CSV
        save_benchmark_data_to_csv_file(csv_filename, "Nanosleep", i, elapsed, jitter, user_cpu_time, system_cpu_time, memory_usage);
    }

    print_stats("Nanosleep", total_jitter, max_jitter, min_jitter);
}

// Benchmark function for signal latency
void benchmark_signal_latency() {
    long long allLatencies[ITERATIONS];
    long long total_latency = 0;
    long long max_latency = 0;
    long long min_latency = LLONG_MAX;

    signal(SIGUSR1, signal_handler);

    for (int i = 0; i < ITERATIONS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        kill(getpid(), SIGUSR1);
        while (!signal_received);

        long long latency = (end.tv_sec - start.tv_sec) * NS_PER_SEC + (end.tv_nsec - start.tv_nsec);
        total_latency += latency;
        allLatencies[i] = latency;
        if (latency > max_latency) max_latency = latency;
        if (latency < min_latency) min_latency = latency;

        signal_received = 0;

        // Get system metrics
        long user_cpu_time, system_cpu_time, memory_usage;
        get_system_metrics(&user_cpu_time, &system_cpu_time, &memory_usage);

        // Save data to CSV
        save_benchmark_data_to_csv_file(csv_filename, "Signal Latency", i, latency, latency - sleep_time.tv_nsec, user_cpu_time, system_cpu_time, memory_usage);
    }

    print_stats("Signal Latency", total_latency, max_latency, min_latency);
}

// Benchmark function for usleep
void benchmark_usleep() {
    long long total_jitter = 0; 
    long long max_jitter = 0;
    long long min_jitter = LLONG_MAX;
    useconds_t sleep_time = 1000; // 1 ms

    for (int i = 0; i < ITERATIONS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        usleep(sleep_time);
        clock_gettime(CLOCK_MONOTONIC, &end);

        long long elapsed = (end.tv_sec - start.tv_sec) * NS_PER_SEC + (end.tv_nsec - start.tv_nsec);
        long long jitter = elapsed - sleep_time * 1000;

        total_jitter += llabs(jitter);
        if (jitter > max_jitter) max_jitter = jitter;
        if (jitter < min_jitter) min_jitter = jitter;

        // Get system metrics
        long user_cpu_time, system_cpu_time, memory_usage;
        get_system_metrics(&user_cpu_time, &system_cpu_time, &memory_usage);

        // Save data to CSV
        save_benchmark_data_to_csv_file(csv_filename, "Usleep", i, elapsed, jitter, user_cpu_time, system_cpu_time, memory_usage);
    }

    print_stats("Usleep", total_jitter, max_jitter, min_jitter);
}

int main() {
    // Add CSV header to the file
    FILE *file = fopen(csv_filename, "w");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "method,iteration,latency_ns,jitter_ns,user_cpu_time,system_cpu_time,memory_usage\n");
    fclose(file);

    configure_realtime_scheduling();
    lock_memory();

    benchmark_nanosleep();
    benchmark_signal_latency();
    benchmark_usleep();

    return 0;
}