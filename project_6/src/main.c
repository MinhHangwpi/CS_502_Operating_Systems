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
#include "thread.h"
#include "readmode.h"
#include "mmapmode.h"

int MAXTHREADS = 16;

int main(int argc, char *argv[])
{
    int fd;
    int mmap_flag;
    size_t read_size = -1; // size of bytes to use on each loop when reading using read();

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <file> <size | mmap> <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = get_num_threads(argv[3]);
    get_size_or_mmap(argv[2], &read_size, &mmap_flag);

    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    if (read_size == -1 && mmap_flag)
    {
        printf("starting mmap mode with %d threads\n", num_threads);
        mmap_mode(fd, num_threads);
    }
    else
    {
        printf("starting read mode with read size: %ld bytes\n", read_size);
        read_mode(fd, read_size);
    }

    return 0;
}
