#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 255
#define MAX_ARGS 20
#define MAX_PATHS 10

char error_message[30] = "An error has occurred\n";


/* 
The following source code is for RUSH (Rapid Unix SHell)
Written by: Aidan Khalil | Developed on 02/01/2024  
Implemented with C, Make, and Ubuntu's Windows Subsystem for Linux (WSL)
*/

void print_error() {
    write(STDERR_FILENO, error_message, strlen(error_message));
    fflush(stderr);
}

void execute_command(char* args[], char* paths[], char* output_file) {
    pid_t pid = fork();

    if (pid < 0) {
        print_error();
    } else if (pid == 0) {
        // child process
        if (output_file != NULL) {
            // redirect standard output to the specified file
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0) {
                print_error();
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        for (int i = 0; paths[i] != NULL; i++) {
            char executable[MAX_INPUT_SIZE];
            strcpy(executable, paths[i]);
            strcat(executable, "/");
            strcat(executable, args[0]);

            if (access(executable, X_OK) == 0) {
                execv(executable, args);
                // If execv fails, print error and exit
                print_error();
                exit(1);
            }
        }

        // If control reaches here, the command was not found
        print_error();
        exit(1);
    } else {
        // Parent process
        wait(NULL);
    }
}

void execute_parallel_commands(char* command_sets[], char* paths[], char* output_file) {
    pid_t pids[MAX_ARGS];
    int i = 0;
    int num_pids = 0;

    while (command_sets[i] != NULL) {
        char* commands[MAX_ARGS];
        int j = 0;

        // Tokenize the command set
        char* token = strtok(command_sets[i], " ");
        while (token != NULL) {
            commands[j++] = token;
            token = strtok(NULL, " ");
        }
        commands[j] = NULL;

        // fork() & execute the command
        pid_t pid = fork();
        if (pid < 0) {
            print_error();
        } else if (pid == 0) {
            execute_command(commands, paths, output_file);
            exit(0);
        } else {
            pids[num_pids++] = pid;
        }

        i++;
    }

    // wait for all child processes to finish!
    for (int k = 0; k < num_pids; k++) {
        waitpid(pids[k], NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    // check if the program is run with any arguments
    if (argc > 1) {
        print_error();
        exit(1);
    }

    char* paths[MAX_PATHS];
    paths[0] = "/bin";
    paths[1] = NULL;

    while (1) {
        printf("rush> ");
        fflush(stdout);

        char input[MAX_INPUT_SIZE];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // handle end of input (for example, Ctrl+D).
            break;
        }

        // remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        // check if the command is empty
        if (strcmp(input, "") == 0) {
            continue;
        }

        // check if the command is exit
        if (strcmp(input, "exit") == 0) {
            exit(0);
        }

        // check if the command is cd
        if (strncmp(input, "cd", 2) == 0) {
            char* token = strtok(input, " ");
            token = strtok(NULL, " ");
            if (token == NULL) {
                print_error();
            } else {
                if (chdir(token) != 0) {
                    print_error();
                }
            }
            continue;
        }

        // check if the command is path
        if (strncmp(input, "path", 4) == 0) {
            char* token = strtok(input, " ");
            int i = 0;
            while ((token = strtok(NULL, " ")) != NULL) {
                paths[i++] = token;
            }
            paths[i] = NULL;
            continue;
        }

        // check if the command is a parallel command with redirection
        if (strstr(input, "&") != NULL) {
            char* command_sets[MAX_ARGS];
            int i = 0;

            // tokenize command sets
            char* token = strtok(input, "&");
            while (token != NULL) {
                command_sets[i++] = token;
                token = strtok(NULL, "&");
            }
            command_sets[i] = NULL;

            // check for redirection in the last command set
            char* output_file = NULL;
            if (command_sets[i - 2] != NULL && strcmp(command_sets[i - 2], ">") == 0) {
                output_file = command_sets[i - 1];
            }

            // execute parallel commands
            execute_parallel_commands(command_sets, paths, output_file);
        } else {
            // tokenize the command
            char* args[MAX_ARGS];
            int i = 0;
            char* token = strtok(input, " ");
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            // check for redirection in the command
            char* output_file = NULL;
            if (args[i - 2] != NULL && strcmp(args[i - 2], ">") == 0) {
                output_file = args[i - 1];
            }

            // execute the command
            execute_command(args, paths, output_file);
        }
    }

    return 0;
}
