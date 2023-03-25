#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_PROCESSES 150
#define MAX_ARGUMENTS 10
#define MAX_COMMAND_LENGTH 500

#define CHECK_NULL(ptr)                               \
    if (!(ptr))                                       \
    {                                                 \
        printf("Error: Memory allocation failed.\n"); \
        exit(1);                                      \
    }

#define CHECK_NEGATIVE(num)                  \
    if ((num) < 0)                           \
    {                                        \
        printf("Error: %s failed.\n", #num); \
        exit(1);                             \
    }

void parse_command_line_arguments(int argc, char *argv[], int *quantum, char *processes[MAX_PROCESSES][MAX_ARGUMENTS], int *num_processes)
{
    // Read the quantum and process command lines from command line arguments.
    if (argc >= 2)
    {
        *quantum = atoi(argv[1]);

        for (int i = 2; i < argc; i++)
        {
            char command_line[MAX_COMMAND_LENGTH];
            strcpy(command_line, argv[i]);
            char *token = strtok(command_line, " ");
            int j = 0;

            while (token != NULL)
            {
                processes[*num_processes][j] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
                CHECK_NULL(processes[*num_processes][j]);
                strcpy(processes[*num_processes][j], token);
                j++;
                token = strtok(NULL, " ");
            }
            (*num_processes)++;
        }
    }
    else
    {
        printf("Missing quantum arguments\n");
        exit(1);
    }
    return;
}

int main(int argc, char *argv[])
{
    int quantum;
    char *processes[MAX_PROCESSES][MAX_ARGUMENTS];
    int num_processes = 0;

    parse_command_line_arguments(argc, argv, &quantum, processes, &num_processes);

    // Run the processes in a Round Robin scheduling.
    int running_processes = num_processes;
    int current_process = 0;

    while (running_processes > 0)
    {
        // Skip completed processes.
        if (processes[current_process][0] == NULL)
        {
            current_process = (current_process + 1) % num_processes;
            continue;
        }

        // Launch process.
        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process.
            execvp(processes[current_process][0], processes[current_process]);

            // If execvp returns, it means an error occurred.
            printf("Error executign command %s\n", processes[current_process][0]);
            exit(0);
        }
        else if (pid > 0)
        {
            // Parent process.
            int sleep_result = usleep(quantum * 1000);
            CHECK_NEGATIVE(sleep_result);

            int kill_result = kill(pid, SIGSTOP);
            CHECK_NEGATIVE(kill_result);

            int status;
            int waitpid_result = waitpid(pid, &status, WUNTRACED);

            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                // Process completed or was terminated.
                for (int i = 0; i < MAX_ARGUMENTS; i++)
                {
                    free(processes[current_process][i]);
                    processes[current_process][i] = NULL;
                }
                running_processes--;
            }
            current_process = (current_process + 1) % num_processes;
        }
        else
        {
            // Error forking.
            printf("Error forking process.\n");
            exit(1);
        }
    }
    return 0;
}