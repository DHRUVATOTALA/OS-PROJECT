#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 500

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

struct ThreadData {
    int row;
};

// Function executed by each thread
void* multiplyRow(void* arg) {
    struct ThreadData* data = (struct ThreadData*) arg;
    int row = data->row;

    for (int col = 0; col < SIZE; col++) {
        C[row][col] = 0;
        for (int k = 0; k < SIZE; k++) {
            C[row][col] += A[row][k] * B[k][col];
        }
    }
    pthread_exit(NULL);
}

// Function to initialize matrices
void initializeMatrices() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
}

// Function to print the result matrix (for small sizes)
void printMatrix() {
    printf("Result Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
}

int main() {
    pthread_t threads[SIZE];
    struct ThreadData threadData[SIZE];

    initializeMatrices();

    clock_t start = clock(); // Start time

    // Creating threads for each row
    for (int i = 0; i < SIZE; i++) {
        threadData[i].row = i;
        pthread_create(&threads[i], NULL, multiplyRow, (void*)&threadData[i]);
    }

    // Joining threads
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock(); // End time
    double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Uncomment below if you want to print the matrix
    // printMatrix();

    printf("Execution Time: %f seconds\n", time_taken);

    return 0;
}

