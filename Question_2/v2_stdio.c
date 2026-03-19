#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) return 1;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    FILE *f_in = fopen(argv[1], "rb");
    FILE *f_out = fopen(argv[2], "wb");
    
    char buffer[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, f_in)) > 0) {
        fwrite(buffer, 1, bytes, f_out);
    }

    fclose(f_in);
    fclose(f_out);

    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("Standard I/O copy time: %f seconds\n", time_taken);

    return 0;
}
