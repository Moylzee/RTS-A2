#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 10000000  // 10 million elements

// Comparison function for qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    srand(time(NULL));  // Seed for random numbers

    // Allocate memory for 10 million integers
    int *data = malloc(DATA_SIZE * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Generate random integers
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = rand() % 1000000;  // Values between 0 and 999999
    }

    // Measure sorting time
    printf("Sorting %d elements...\n", DATA_SIZE);
    clock_t start = clock();
    qsort(data, DATA_SIZE, sizeof(int), compare);
    clock_t end = clock();

    // Compute time taken
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sorting completed in %.2f seconds.\n", time_taken);

    // Free allocated memory
    free(data);

    return 0;
}