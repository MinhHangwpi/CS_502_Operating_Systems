#ifndef THREAD_H_
#define THREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "argparse.h"
#include "config.h"

extern int MAXTHREADS;

typedef struct
{
    char *start;
    char *end; // if -1, indicates the string continues beyond the current chunk
} StringRange;

typedef struct
{
    int thread_id;
    char *start;
    char *end;
    size_t chunk_size;
    int num_strings;
    StringRange *ranges;
    int is_last_thread;
} ThreadData;

void *thread_func(void *arg);

ThreadData *init_thread_data(int thread_id, char *start, char *end, size_t chunk_size, int is_last);

#endif