#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>


struct background_task bg_tasks[MAX_BG_TASKS]; // Array to hold background tasks
int bg_task_count = 0; // number of background tasks

void print_resource_usage_stats(struct timeval start_time, struct timeval end_time, struct rusage usage){
    long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000;
    elapsed_time += (end_time.tv_usec - start_time.tv_usec) / 1000;  // in miliseconds

    long user_time = (usage.ru_utime.tv_sec * 1000) + (usage.ru_utime.tv_usec / 1000);
    long system_time = (usage.ru_stime.tv_sec * 1000) + (usage.ru_stime.tv_usec / 1000);

    printf("CPU Time Used (User + System): %ldms\n", user_time + system_time);
    printf("Elapsed Wall-Clock Time: %ldms\n", elapsed_time);
    printf("Involuntary Context Switches: %ldms\n", usage.ru_nivcsw);
    printf("Voluntary Context Switches: %ldms\n", usage.ru_nvcsw);
    printf("Major Page Faults: %ld\n", usage.ru_majflt);
    printf("Minor Page Faults: %ld\n", usage.ru_minflt);
    printf("Maximum Resident Set Size: %ldKB\n", usage.ru_maxrss);
}


void execute_command(char *argv[], int background_flag) {
    struct timeval start_time, end_time;
    struct rusage usage;

    gettimeofday(&start_time, NULL);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    } else {
        if (background_flag){
            // add to background tasks list
            bg_tasks[bg_task_count].pid = pid;
            strcpy(bg_tasks[bg_task_count].command, argv[0]);
            bg_tasks[bg_task_count].start_time = start_time;
            bg_task_count++;
            printf("[%d] %d\n", bg_task_count, pid);
        } else {
            int status;
            waitpid(pid, &status, 0);
            gettimeofday(&end_time, NULL);
            getrusage(RUSAGE_CHILDREN, &usage);
            print_resource_usage_stats(start_time, end_time, usage);
        }
    }
}


/**
check_background_tasks: handle removal of completed tasks from the bg_tasks array
shifting the remaining tasks in the array to fill the gap left by the completed task
*/

void check_background_tasks() {
    struct timeval end_time;
    struct rusage usage;

    int i = 0;
    while (i < bg_task_count) {
        int status;
        pid_t result = waitpid(bg_tasks[i].pid, &status, WNOHANG); // return immediately event if no child has exited

        if (result > 0) {
            // if result is any integer greater than 0, meaning the child process has completed. 
            printf("[%d] %d %s - Completed\n", i+1, bg_tasks[i].pid, bg_tasks[i].command);

            // printing out stats of the completed background task
            gettimeofday(&end_time, NULL);
            getrusage(RUSAGE_CHILDREN, &usage);
            print_resource_usage_stats(bg_tasks[i].start_time, end_time, usage);

            // Shift the remaining tasks forward to fill the gap
            for (int j = i; j < bg_task_count - 1; j++) {
                bg_tasks[j] = bg_tasks[j + 1];
            }
            bg_task_count--; // Decrease the count of background tasks
        } else {
            i++; // Only increment if no task was removed
        }
    }
}

void print_all_background_tasks(){
    for (int i=0; i < bg_task_count; i++){
        printf("[%d] %d %s\n", i+1, bg_tasks[i].pid, bg_tasks[i].command);
    }
}
