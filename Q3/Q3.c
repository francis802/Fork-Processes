#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


int main(int argc, char* argv[]){
    
    // Check if the user has entered the correct number of arguments
    if (argc != 4) {
        printf("correct usage: tokenring n_pipes probability time\n");
        return EXIT_FAILURE;
    }

    // Global variables
    int n = atoi(argv[1]);
    int token = 0;
    int sleep_time = atoi(argv[3]);

    double probability = atof(argv[2]);

    //Pipe variables
    char *curr_pipe = (char*) malloc(sizeof(char) * 9+(strlen(argv[1])-1)*2); //Always alloc 9 bytes of memory + enough bytes for the biggest string possible (example: n=11, biggestCase: pipe10to11)
    char *WRITE_END = (char*) malloc(sizeof(char) * 9+(strlen(argv[1])-1)*2);
    char *READ_END = (char*) malloc(sizeof(char) * 9+(strlen(argv[1])-1)*2);
    
    pid_t pid_array[n];

    // Create the pipes
    for (int i = 0; i<n;i++){
        sprintf(curr_pipe, "pipe%dto%d", i+1, (i+1)%n+1);
        if((mkfifo(curr_pipe, 0666)) < 0) {
            perror("mkfifo");
            return EXIT_FAILURE;
        }
    }

    free(curr_pipe);
    
    for(int i = 0; i<n; i++){
        int process_number = i+1;
        pid_array[i] = fork();
        if (pid_array[i] < 0) {
            //Error forking
            perror("fork failed");
            return EXIT_FAILURE;
        }
        else if(pid_array[i] == 0) {
            if(i == n-1) { //Last process, create the pipe connected to the first process
                sprintf(WRITE_END, "pipe%dto1", process_number);
                sprintf(READ_END, "pipe%dto%d", process_number-1, process_number);
            } else if(i == 0) { //First process, create the pipe connected to the last process
                sprintf(WRITE_END, "pipe%dto%d", process_number, process_number+1);
                sprintf(READ_END, "pipe%dto1", n);
            } else { //All other processes
                sprintf(WRITE_END, "pipe%dto%d", process_number, process_number+1);
                sprintf(READ_END, "pipe%dto%d", process_number-1, process_number);
            }
        
            srand(time(NULL) - i); //Seed the random number generator 

            int pipe_fd[2]; //File descriptors for the pipe

            if(i == 0){ //First process, open the pipe for writing so that the 2nd process can read from it and doesn't deadlock
                pipe_fd[0] = open(WRITE_END, O_WRONLY);
                if(pipe_fd[0] < 0) {
                    perror("pipe open1");
                    return EXIT_FAILURE;
                }

                token++;

                write(pipe_fd[0], &token, sizeof(int));
                if(pipe_fd[0] < 0) {
                    perror("pipe write1");
                    return EXIT_FAILURE;
                }
                close(pipe_fd[0]);
            }


            while(1){ //Infinite loop to keep the process cycle running
                pipe_fd[1] = open(READ_END, O_RDONLY);
                if(pipe_fd[1] < 0) { //Open the pipe for reading
                    perror("pipe open2");
                    return EXIT_FAILURE;
                }

                read(pipe_fd[1], &token, sizeof(int));
                if(pipe_fd[1] < 0) { //Read the token from the pipe
                    perror("pipe read1");
                    return EXIT_FAILURE;
                }

                close(pipe_fd[1]);

                token++;

                if((double)rand() / (double)RAND_MAX < probability){ //If the random number is less than the probability, sleep for the specified time
                    printf("[p%d] lock on token (val = %d)\n", process_number, token);
                    sleep(sleep_time);
                    printf("[p%d] unlock token\n", process_number);
                }

                pipe_fd[0] = open(WRITE_END, O_WRONLY);
                if(pipe_fd[0] < 0) { //Open the pipe for writing
                    perror("pipe open3");
                    return EXIT_FAILURE;
                }

                write(pipe_fd[0], &token, sizeof(int));
                if(pipe_fd[0] < 0) { //Write the token to the pipe
                    perror("pipe write2");
                    return EXIT_FAILURE;
                }
                close(pipe_fd[0]);
            }
            return EXIT_SUCCESS;
        }
    }
    for(int i = 0; i < n; i++) { //This allows to stop the program with Ctrl+C, otherwise the program would keep running because child porcesses become zombies
        if(waitpid(pid_array[i], NULL, 0) < 0) {
            perror("waitpid");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}