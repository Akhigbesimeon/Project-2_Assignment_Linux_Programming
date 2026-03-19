#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char **file_list;
int num_files;
int current_file_index = 0;
char *keyword;
FILE *output_file;

// Mutex for retrieving the next file to process
pthread_mutex_t index_mutex = PTHREAD_MUTEX_INITIALIZER;
// Mutex for writing safely to the output file
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;

// Processes next file for keyword
void* search_files(void* arg) {
    while (1) {
        pthread_mutex_lock(&index_mutex);
        if (current_file_index >= num_files) {
            pthread_mutex_unlock(&index_mutex);
            break; 
        }
        int my_index = current_file_index++;
        pthread_mutex_unlock(&index_mutex);

        FILE *f = fopen(file_list[my_index], "r");
        if (!f) {
            continue;
        }

        int count = 0;
        char line[2048];
        while (fgets(line, sizeof(line), f)) {
            char *ptr = line;
            while ((ptr = strstr(ptr, keyword)) != NULL) {
                count++;
                ptr += strlen(keyword); 
            }
        }
        fclose(f);

        // Safely write results
        pthread_mutex_lock(&write_mutex);
        fprintf(output_file, "File: %s - Occurrences: %d\n", file_list[my_index], count);
        pthread_mutex_unlock(&write_mutex);
    }
    return NULL;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: ./search keyword output.txt file1.txt file2.txt ... <number_of_threads>\n");
        return 1;
    }

    keyword = argv[1];
    output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("Failed to open output file");
        return 1;
    }

    int num_threads = atoi(argv[argc - 1]);
    num_files = argc - 4; 
    file_list = &argv[3]; 

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));

    // Execute threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, search_files, NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(output_file);
    free(threads);
    
    printf("Search complete. Check %s for results.\n", argv[2]);

    return 0;
}
