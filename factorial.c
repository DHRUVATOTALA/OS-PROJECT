#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define MAX_DIGITS 500  // To store large factorials
#define NUM_THREADS 4   // Number of threads

int result[MAX_DIGITS];  // Array to store factorial digits
int n = 100;             // Change as needed

typedef struct {
    int start, end;
} ThreadData;

// Function to multiply result[] by a number
void multiply(int num) {
    int carry = 0;
    for (int i = 0; i < MAX_DIGITS; i++) {
        int prod = result[i] * num + carry;
        result[i] = prod % 10;  // Store last digit
        carry = prod / 10;      // Carry for next iteration
    }
}

// Thread function to compute factorial in chunks
void *factorial_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i <= data->end; i++) {
        multiply(i);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    memset(result, 0, sizeof(result));
    result[0] = 1;  // 0! = 1

    clock_t start_time = clock();  // Start timing

    int chunk_size = n / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk_size + 1;
        thread_data[i].end = (i == NUM_THREADS - 1) ? n : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, factorial_thread, (void *)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();  // End timing

    // Print the factorial result
    printf("Factorial of %d is: ", n);
    int started = 0;
    for (int i = MAX_DIGITS - 1; i >= 0; i--) {
        if (result[i] != 0) started = 1;
        if (started) printf("%d", result[i]);
    }
    printf("\n");

    // Print execution time
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", time_taken);

    return 0;
}

