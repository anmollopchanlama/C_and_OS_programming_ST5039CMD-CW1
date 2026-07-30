#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>

typedef struct {
    pid_t child_pid;
    atomic_int is_active;
    long time_limit;       
    long memory_limit;     
} SandboxMonitor;

void* runtime_telemetry_thread(void* arg) {
    SandboxMonitor *monitor = (SandboxMonitor*)arg;
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", monitor->child_pid);

    while (atomic_load(&monitor->is_active)) {
        FILE *f = fopen(path, "r");
        if (!f) break; 

        unsigned long utime, stime;
        long rss_pages;
        int parsed = fscanf(f, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %*u %*u %ld", 
                            &utime, &stime, &rss_pages);
        fclose(f);

        if (parsed >= 3) {
            long page_size = sysconf(_SC_PAGESIZE);
            long current_memory = rss_pages * page_size;

            printf("[Sandbox Telemetry] Child RSS Memory: %ld KB\n", current_memory / 1024);

            if (current_memory > monitor->memory_limit) {
                printf("[!] Policy Enforcement: Memory Limit Exceeded! Sending SIGKILL.\n");
                kill(monitor->child_pid, SIGKILL);
                break;
            }
        }
        usleep(250000); 
    }
    return NULL;
}

void* execution_timeout_thread(void* arg) {
    SandboxMonitor *monitor = (SandboxMonitor*)arg;
    long elapsed = 0;

    while (atomic_load(&monitor->is_active)) {
        if (elapsed >= monitor->time_limit) {
            printf("[!] Policy Enforcement: Execution Timeout Reached! Sending SIGKILL.\n");
            kill(monitor->child_pid, SIGKILL);
            break;
        }
        sleep(1);
        elapsed++;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    SandboxMonitor monitor;
    pid_t pid;
    pthread_t telemetry_tid, timeout_tid;
    int status;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path-to-untrusted-binary> [args...]\n", argv[0]);
        return 1;
    }

    monitor.time_limit = 4;                 
    monitor.memory_limit = 12 * 1024 * 1024; 
    atomic_init(&monitor.is_active, 1);

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        struct rlimit limit;
        limit.rlim_cur = monitor.memory_limit;
        limit.rlim_max = monitor.memory_limit;
        setrlimit(RLIMIT_AS, &limit); 

        // Correct pointer offset array: slides window over to target binary elements smoothly
        char *binary_path = argv[1];
        char **child_args = &argv[1];
        char *env[] = {NULL};
        
        execve(binary_path, child_args, env);
        perror("execve failure loading binary");
        exit(1);
    } else {
        monitor.child_pid = pid;

        pthread_create(&telemetry_tid, NULL, runtime_telemetry_thread, &monitor);
        pthread_create(&timeout_tid, NULL, execution_timeout_thread, &monitor);

        waitpid(pid, &status, 0);
        atomic_store(&monitor.is_active, 0); 

        pthread_join(telemetry_tid, NULL);
        pthread_join(timeout_tid, NULL);

        printf("[+] Sandbox Controller Finished. Child status: ");
        if (WIFSIGNALED(status)) {
            printf("Killed by kernel signal %d\n", WTERMSIG(status));
        } else {
            printf("Normal termination with exit code %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
