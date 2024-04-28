#include "argparse.h"

int get_num_threads(const char *arg)
{
    char *cptr = arg;
    int len = 0;
    int num_threads = 0;

    while (*cptr != '\0' && len < 3)
    {
        if (isdigit(*cptr))
        {
            num_threads = num_threads * 10 + (*cptr - '0');
        }
        cptr++;
        len++;
    }
    return num_threads;
}

void get_size_or_mmap(const char *arg, size_t *size, int *mmap_flag)
{
    char *cptr = arg;
    if (strcmp(cptr, "mmap") == 0)
    {
        *mmap_flag = 1;
        return;
    }

    char *endptr;
    long val = strtol(cptr, &endptr, 10);

    *size = val;
    *mmap_flag = 0;
    
    if (*endptr != '\0' || errno == ERANGE || val < 1 || val > MAX_READ_SIZE)
    {
        fprintf(stderr, "Invalid chunk size. Please specify a valid integer or 'mmap'.\n");
        return;
    }
}