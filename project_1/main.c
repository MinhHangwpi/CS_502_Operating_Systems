#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <string.h>

#include "stack.h"


int main(int argc, char* argv[]){
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARGS];
    char prompt[50] = "==> ";

    if (argc < 2){
        // acting like a shell
        while (1) {
            printf("%s", prompt);
            if (!fgets(input, MAX_INPUT_LENGTH, stdin)){
                printf("\n");
                break; //EOF detected
            }

            if (input[strlen(input) - 1] == '\n'){
                input[strlen(input) - 1] = '\0'; // remove new line character
            }

            // Parse input into arguments
            int arg_count = 0;
            args[arg_count] = strtok(input, " ");
            while (args[arg_count] != NULL && arg_count < MAX_ARGS - 1){
                args[++arg_count] = strtok(NULL, " ");
            }

            // Check for and handle background task indicator (&)
            int background = 0;
            if (arg_count > 0 && strcmp(args[arg_count - 1], "&") == 0) {
                background = 1;
                args[arg_count - 1] = NULL; // Remove '&' from arguments
            }


            // check for built-in commands
            if (args[0] == NULL){
                continue; // empty command
            } else if (strcmp(args[0], "exit") == 0){
                if (bg_task_count > 0){
                    printf("Cannot exit the shell: %d running background jobs\n", bg_task_count);
                    continue;
                }
                break; // exit shell if no running background tasks
            } else if (strcmp(args[0], "cd") == 0){
                if (args[1] != NULL){
                    if (chdir(args[1]) != 0){
                        perror("chdir");
                    }
                    // Successfully changed directory, now get and print the new directory
                    char cwd[1024];
                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                        printf("Changed to directory: %s\n", cwd);
                    } else {
                        perror("getcwd");
                    }
                } else {
                    fprintf(stderr, "cd: missing arguments]\n");
                }
            } else if (strcmp(args[0], "set") == 0 && args[1] && strcmp(args[1], "prompt") == 0 && args[2] && strcmp(args[2], "=") == 0) {
                if (args[3] != NULL) {
                    strncpy(prompt, args[3], sizeof(prompt));
                    prompt[sizeof(prompt) - 1] = '\0';
                } else {
                    fprintf(stderr, "set prompt: missing argument\n");
                }
            } else if (strcmp(args[0], "jobs") == 0){
                print_all_background_tasks();
            } else {
                // if not built-in commands, just execute as usual
                execute_command(args, background);
            }

            check_background_tasks();
        }
    } else {
        // just executing the command instead of acting like a shell 
        execute_command(&argv[1], 0);
    }
    return 0;
}