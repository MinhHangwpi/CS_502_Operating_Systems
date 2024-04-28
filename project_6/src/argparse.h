#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#include "config.h"

int get_num_threads(const char * arg);
void get_size_or_mmap(const char * arg, size_t * size, int * mmap_flag);