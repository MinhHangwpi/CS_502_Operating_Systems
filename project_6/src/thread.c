#include "thread.h"

void *thread_func(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    int allocated_strings = 1000; // Start with space for 1000 string ranges
    data->num_strings = 0;

    data->ranges = malloc(sizeof(StringRange) * allocated_strings);
    if (!data->ranges)
    {
        perror("Failed to allocate string ranges");
        free(data);
        return NULL;
    }

    char *start_string = NULL;
    char *current_pos = data->start;

    while (current_pos < data->end)
    {
        if (isprint(*current_pos))
        {
            if (!start_string)
                start_string = current_pos;
        }
        else if (start_string)
        {
            // Check if there is enough space and resize if necessary
            if (data->num_strings >= allocated_strings)
            {
                allocated_strings *= 2;
                StringRange *new_ranges = realloc(data->ranges, sizeof(StringRange) * allocated_strings);
                if (!new_ranges)
                {
                    perror("Failed to reallocate string ranges");
                    free(data->ranges);
                    return NULL;
                }
                data->ranges = new_ranges;
            }

            data->ranges[data->num_strings].start = start_string;
            data->ranges[data->num_strings].end = current_pos;
            data->num_strings++;
            start_string = NULL;
        }
        current_pos++;
    }

    // Handle potential ongoing string at the end of the chunk
    if (start_string)
    {
        if (data->num_strings == allocated_strings)
        {
            allocated_strings += 1; // Just add one more space if needed
            StringRange *new_ranges = realloc(data->ranges, sizeof(StringRange) * allocated_strings);
            if (!new_ranges)
            {
                perror("Failed to reallocate string ranges");
                free(data->ranges);
                return NULL;
            }
            data->ranges = new_ranges;
        }
        data->ranges[data->num_strings].start = start_string;
        // For the last thread, mark the end as the last printable character
        data->ranges[data->num_strings].end = (data->is_last_thread) ? current_pos : NULL;
        data->num_strings++;
    }

    return NULL;
}

ThreadData *init_thread_data(int thread_id, char *start, char *end, size_t chunk_size, int is_last)
{
    ThreadData *td = malloc(sizeof(ThreadData));
    if (!td)
    {
        perror("Failed to allocate ThreadData");
        return NULL;
    }

    td->thread_id = thread_id;
    td->start = start;
    td->end = end;
    td->chunk_size = chunk_size;
    td->is_last_thread = is_last;

    return td;
}
