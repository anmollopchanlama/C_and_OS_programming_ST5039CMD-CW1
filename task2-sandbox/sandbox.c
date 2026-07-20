/*
 * sandbox.c
 *
 * Basic idea for Task 2: this program takes an untrusted binary as input,
 * runs it in a child process, and watches it from the parent side (time,
 * resource use, etc). If it misbehaves or runs too long, parent kills it
 * using signals. Will add pthreads later for monitoring concurrently.
 *
 * Not implemented yet, just the outline for now - fork/execve logic
 * coming in next session.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <path to binary to sandbox>\n", argv[0]);
        return 1;
    }

    // next: fork() here, child runs execve(argv[1]), parent waits/monitors
    // still need to figure out timeout handling and signal-based kill

    return 0;
}
