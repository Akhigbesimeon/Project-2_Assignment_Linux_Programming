#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) return 1;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int fd_in = open(argv[1], O_RDONLY);
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    char buffer[BUFFER_SIZE];
    ssize_t bytes;
    while ((bytes = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        write(fd_out, buffer, bytes);
    }

    close(fd_in);
    close(fd_out);

    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("Low-level copy time: %f seconds\n", time_taken);

    return 0;
}
