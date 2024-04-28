#include "mmapmode.h"

void mmap_mode(int fd, int num_threads)
{
    char *pchFile;
    struct stat sb;

    if (fstat(fd, &sb) < 0)
    {
        perror("Could not stat file to obtain its size");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_threads];
    ThreadData *threadData[num_threads];

    pchFile = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (pchFile == MAP_FAILED)
    {
        exit(EXIT_FAILURE);
    }

    /* Divide the file size evenly among all threads and process */
    size_t chunkSize = sb.st_size / num_threads;

    for (int i = 0; i < num_threads; i++)
    {

        int thread_id = i;
        char *start = pchFile + i * chunkSize;
        size_t chunk_size = (i == num_threads - 1) ? (sb.st_size - i * chunkSize) : chunkSize;
        char *end = start + chunk_size;
        int is_last_thread = (i == num_threads - 1) ? 1 : 0;

        threadData[i] = init_thread_data(thread_id, start, end, chunk_size, is_last_thread);

        int ret = pthread_create(&threads[i], NULL, thread_func, threadData[i]);
        if (ret != 0)
        {
            fprintf(stderr, "Error creating thread %d: %s\n", i, strerror(ret));
        }
    }

    printf("File size: %ld bytes\n", sb.st_size);

    for (int i = 0; i < num_threads; i++)
    {
        int join_ret = pthread_join(threads[i], NULL);
        if (join_ret != 0)
        {
            fprintf(stderr, "Error joining thread %d: %s\n", i, strerror(join_ret));
        }
    }

    combine_output(threadData, num_threads);

    // Free memory after combining output
    for (int i = 0; i < num_threads; i++)
    {
        free(threadData[i]->ranges);
        free(threadData[i]);
    }

    /* Unmap the file from memory and close the file descriptor */
    if (munmap(pchFile, sb.st_size) < 0)
    {
        perror("Could not unmap memory");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void combine_output(ThreadData **threadData, int num_threads)
{
    int total_num_strings = 0;
    int max_string_length = 0;
    int current_string_length = 0;

    for (int i = 0; i < num_threads; i++)
    {
        int num_strings_in_thread = threadData[i]->num_strings;

        if (num_strings_in_thread == 0)
            current_string_length = 0;

        for (int j = 0; j < num_strings_in_thread; j++)
        {
            if (!threadData[i]->ranges[j].end)
            {
                int len = threadData[i]->end - threadData[i]->ranges[j].start;
                current_string_length += len;
            }
            else
            {
                int len = threadData[i]->ranges[j].end - threadData[i]->ranges[j].start;
                current_string_length += len;

                if (current_string_length >= 4)
                {
                    total_num_strings++;
                }

                max_string_length = (current_string_length > max_string_length) ? current_string_length : max_string_length;

                // reset
                current_string_length = 0;
            }
        }
        // Handle edge case: if the beginning of a chunk is while the previous ends with null then current length is reset
        if (i != 0 && !threadData[i - 1]->ranges[num_strings_in_thread - 1].end && !isprint(*threadData[i]->start))
        {
            current_string_length = 0;
        }
    }

    printf("Strings of at least length 4: %d\n", total_num_strings);
    printf("Maximum string length: %d bytes\n", max_string_length);
}