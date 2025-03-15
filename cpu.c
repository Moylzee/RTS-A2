#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 500  // Adjust size for more CPU load

void multiply_matrices(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void initialize_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 10;  // Random values between 0-9
        }
    }
}

int main() {
    printf("--- STRESSING CPU ---\n");
    srand(time(NULL));

    int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    
    initialize_matrix(A);
    initialize_matrix(B);

    clock_t start = clock();
    multiply_matrices(A, B, C);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication completed in %.3f seconds.\n", time_taken);

    return 0;
}
