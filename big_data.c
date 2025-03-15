#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DATA_SIZE (1024 * 1024 * 256 * 3)  // 256 * 3 million integers (~3GB)
#define ITERATIONS 10  // Number of passes over memory

int main() {
    printf("Allocating %ld MB of memory...\n", (DATA_SIZE * sizeof(int)) / (1024 * 1024));

    // Allocate a large chunk of memory
    int *data = (int *)malloc(DATA_SIZE * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    srand(time(NULL));

    // Write random data to memory
    printf("Filling memory with random numbers...\n");
    for (size_t i = 0; i < DATA_SIZE; i++) {
        data[i] = rand();
    }

    // Perform multiple read/write passes to keep memory in use
    printf("Stressing memory for %d iterations...\n", ITERATIONS);
    for (int iter = 0; iter < ITERATIONS; iter++) {
        printf("Iteration %d...\n", iter + 1);
        for (size_t i = 0; i < DATA_SIZE; i++) {
            data[i] += rand();  // Random writes
        }

        // Random access pattern
        for (size_t i = 0; i < DATA_SIZE / 10; i++) {
            volatile int temp = data[rand() % DATA_SIZE];  // Prevent compiler optimization
            (void)temp;  // Use variable to avoid warnings
        }

        sleep(1);  // Give OS time to track memory load
    }

    printf("Memory stress test completed.\n");

    free(data);
    return 0;
}