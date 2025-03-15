
#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 10000
#define NS_PER_SEC 1000000000

// Function to print benchmark stats
void print_stats(const char *label, long long total, long long max, long long min) {
    printf("\n%s Benchmark:\n"
            "Average: %lld ns\n"
            "Max: %lld ns\n"
            "Min: %lld ns\n", label, total / ITERATIONS, max, min);
}

// Function configure_realtime_scheduling configures the process to run with realtime scheduling
void configure_realtime_scheduling() {
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }
}

// Function lock_memory locks the current and future memory of the process
void lock_memory() {
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
        perror("mlockall");
        exit(EXIT_FAILURE);
    }
}

void save_to_csv_file(const char *filename, long long data[]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ITERATIONS; i++) {
        fprintf(file, "%lld\n", data[i]);
    }

    fclose(file);
}