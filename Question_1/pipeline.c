#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Main function
int main() {
    int pipe_ps_grep[2]; 
    int pipe_grep_parent[2];

    if (pipe(pipe_ps_grep) == -1 || pipe(pipe_grep_parent) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child 1: ps aux
        dup2(pipe_ps_grep[1], STDOUT_FILENO); 
        
        close(pipe_ps_grep[0]); close(pipe_ps_grep[1]);
        close(pipe_grep_parent[0]); close(pipe_grep_parent[1]);
        
        char *args[] = {"ps", "aux", NULL};
        execvp("ps", args);
        perror("execvp ps failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child 2: grep root
        dup2(pipe_ps_grep[0], STDIN_FILENO);   
        dup2(pipe_grep_parent[1], STDOUT_FILENO);
        
        close(pipe_ps_grep[0]); close(pipe_ps_grep[1]);
        close(pipe_grep_parent[0]); close(pipe_grep_parent[1]);
        
        char *args[] = {"grep", "root", NULL};
        execvp("grep", args);
        perror("execvp grep failed");
        exit(EXIT_FAILURE);
    }

    // Parent Process
    close(pipe_ps_grep[0]); close(pipe_ps_grep[1]);
    close(pipe_grep_parent[1]);

    FILE *outfile = fopen("output.txt", "w");
    if (!outfile) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(pipe_grep_parent[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        fprintf(outfile, "%s", buffer);
    }
    fclose(outfile);
    close(pipe_grep_parent[0]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // Read and display part of the output
    printf("-- First 5 lines of captured output --\n");
    outfile = fopen("output.txt", "r");
    for (int i = 0; i < 5 && fgets(buffer, sizeof(buffer), outfile); i++) {
        printf("%s", buffer);
    }
    fclose(outfile);

    return 0;
}
