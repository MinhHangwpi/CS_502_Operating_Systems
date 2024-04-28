#include "readmode.h"

void read_mode(int fd, int chunk_size) {
    char *buffer = malloc(chunk_size);
    if (!buffer) {
        perror("Failed to allocate memory");
        return;
    }

    int read_bytes;
    long total_file_size = 0;
    int total_strings = 0;
    int max_string_length = 0;
    int current_string_length = 0;
    int in_string = 0;

    while ((read_bytes = read(fd, buffer, chunk_size)) > 0) {
        total_file_size += read_bytes;
        for (int i = 0; i < read_bytes; i++) {
            if (isprint(buffer[i])) {
                if (!in_string) {
                    in_string = 1; // Start of a new string
                    current_string_length = 0;
                }
                current_string_length++;
            } else {
                if (in_string && current_string_length >= 4) {
                    total_strings++;
                    if (current_string_length > max_string_length) {
                        max_string_length = current_string_length;
                    }
                }
                in_string = 0; // End of the current string
            }
        }
    }

    // Check if the last chunk ended with an ongoing string
    if (in_string && current_string_length >= 4) {
        total_strings++;
        if (current_string_length > max_string_length) {
            max_string_length = current_string_length;
        }
    }

    free(buffer);

    // Print the results
    printf("File size: %ld bytes\n", total_file_size);
    printf("Strings of at least length 4: %d\n", total_strings);
    printf("Maximum string length: %d bytes\n", max_string_length);

    if (read_bytes == -1) {
        perror("Read error");
    }
}
