# Project-2_Assignment_Linux_Programming

This repository contains four C programs demonstrating core operating system concepts, including inter-process communication (IPC), system call tracing, low-level versus standard file I/O, and multithreaded synchronization using POSIX threads.

## Project Structure

* pipeline.c: Implements process creation and a multi-stage IPC pipeline (ps aux | grep root).
* v1_lowlevel.c: Copies a file using low-level POSIX system calls (read, write).
* v2_stdio.c: Copies a file using standard C library I/O functions (fread, fwrite).
* primes.c: Calculates prime numbers up to 200,000 using 16 synchronized threads.
* search.c: Multithreaded utility to search for a keyword across multiple text files concurrently.

## Compilation Instructions

* gcc -Wall -o pipeline pipeline.c
* gcc -Wall -o v1_lowlevel v1_lowlevel.c
* gcc -Wall -o v2_stdio v2_stdio.c
* gcc -Wall -pthread -o primes primes.c
* gcc -Wall -pthread -o search search.c

## Execution and Testing

### 1. Process Pipeline (pipeline.c)
Executes the pipeline and captures the output to output.txt.
./pipeline

To trace the system calls, use strace:
* strace -f -e trace=process,pipe,file,read,write ./pipeline

### 2. Large File Copy Utility
Before running, generate a 100MB dummy file for testing:
* dd if=/dev/urandom of=large_source.bin bs=1M count=100

Run both versions to compare execution times:
* ./v1_lowlevel large_source.bin dest1.bin
* ./v2_stdio large_source.bin dest2.bin

To profile the system calls:
* strace -c ./v1_lowlevel large_source.bin dest1.bin
* strace -c ./v2_stdio large_source.bin dest2.bin

### 3. Prime Number Counter (primes.c)
* Executes the 16-thread prime counter.
* ./primes

### 4. Concurrent Keyword Search (search.c)
* Requires a target keyword, an output file name, multiple text files to search, and the number of threads.
* Format: ./search <keyword> <output_file> <file1> <file2> ... <thread_count>

Example (using 4 threads):
* ./search "Cat" results.txt log1.txt log2.txt log3.txt 4
