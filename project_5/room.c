#include "room.h"


struct roomconfig roomconfigs[MAXROOMS];

int parse_rooms_config(const char *filename)
{
    FILE *file;
    char line[MAXROOMS];
    int line_count = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file.\n");
        exit(1);
    }

    while (fgets(line, MAXROOMS, file) != NULL)
    {
        line_count++;
        if (line_count > MAXROOMS)
        {
            fclose(file);
            fprintf(stderr, "Error: The number of lines exceeded MAXROOMS: %d.\n", line_count);
            exit(1);
        }

        int token_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL && token_count < 2)
        {
            if (token_count == 0)
            {
                roomconfigs[line_count - 1].capacity = atoi(token);
            }
            else if (token_count == 1)
            {
                roomconfigs[line_count - 1].time = atoi(token);
            }

            token = strtok(NULL, " ");
            token_count++;
        }

        if (token_count != 2)
        {
            fclose(file);
            fprintf(stderr, "Error: Line %d does not contain exactly 2 tokens: %d tokens\n", line_count, token_count);
            exit(1);
        }
    }

    fclose(file);
    return line_count;
}