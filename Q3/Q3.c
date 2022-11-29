//Write a program tokenring that gets 3 integers - n, p and t - from the command line. When executed, it creates n processes connected to each other by “named pipes”. The named pipes are designated pipe1to2, pipe2to3,..., pipento1 and each allows one-way communication between the i-th and the i+1-th processes. The last named pipe closes the ring allowing process n to communicate with process 1. Once this process ring is created, p1 should send a token (a message with an integer with an initial value 0) to the next process (p1 > p2) and so on (p2 > p3 >... > pn > p1 >...). The token must circulate between processes endlessly, increasing its value at each hop. Each time a process receives the token, it must immediately resend it to the next process or, with a probability of p, block its submission during t seconds and print a message alerting to this fact (see the following example). In either case, the value of the token must be incremented
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char* argv[]){
    
    // Check if the user has entered the correct number of arguments
    if (argc != 4) {
        printf("correct usage: tokenring n_pipes probability time\n");
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    char curr_pipe[9+(strlen(argv[1])-1)*2]; //Always alloc 9 bytes of memory + enough bytes for the biggest string possible (example: n=11, biggestCase: pipe10to11)
    int token = 0;
    char stoken[10];
    //char process_number_char[5];
    int fd = 0;
    pid_t pid;
    for (int i = 0; i<n-1;i++){
        snprintf(curr_pipe, sizeof curr_pipe, "pipe%dto%d", i+1, (i+1)%n+1);
        mkfifo(curr_pipe, 0666);
        pid = fork();
        if (pid < 0) {
                //Error forking
                perror("fork failed");
                return EXIT_FAILURE;
        }
        if (pid>0) break;
    }
    int flag = 1;
    while(1){
        if (flag){
            printf("Here with pid: %d\n", getpid());
            flag = 0;
        }
        if(pid > 0){
            fd = open(curr_pipe, O_WRONLY);
            if (fd ==- 1) {
                perror("Open error");
            }
            token++;
            snprintf(stoken, sizeof stoken, "%d",token);
            if (write(fd, stoken, strlen(curr_pipe))==-1) {
                perror("Write error");
                return EXIT_FAILURE;
            }
        }
    }
}