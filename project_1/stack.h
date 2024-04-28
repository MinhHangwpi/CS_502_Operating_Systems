#ifndef STACK_H
#define STACK_H

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

#define MAX_INPUT_LENGTH 128
#define MAX_ARGS 32
#define MAX_BG_TASKS 50

// Define a struct to keep track of background tasks
struct background_task {
    pid_t pid;
    char command[128];
    struct timeval start_time;
};

extern int bg_task_count;


// Function prototypes
void print_resource_usage_stats(struct timeval start_time, struct timeval end_time, struct rusage usage);
void execute_command(char *argv[], int background_flag);
void check_background_tasks();
void print_all_background_tasks();

#endif // STACK_H
