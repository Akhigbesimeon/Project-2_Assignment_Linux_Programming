#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 16
#define MAX_NUM 200000

// Stores final count of prime numbers
int total_primes = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Checks if it's a prime number
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

// Finds primes in a subrange
void* count_primes(void* arg) {
    int thread_id = *(int*)arg;
    int range = MAX_NUM / NUM_THREADS;

    int start = thread_id * range + 1;
    int end = (thread_id == NUM_THREADS - 1) ? MAX_NUM : (thread_id + 1) * range;

    int local_count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    total_primes += local_count;
    pthread_mutex_unlock(&mutex);

    free(arg);
    return NULL;
}

// Main function
int main() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, count_primes, id);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("The synchronized total number of prime numbers between 1 and %d is %d\n", MAX_NUM, total_primes);

    return 0;
}
