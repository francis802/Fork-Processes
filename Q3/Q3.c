//Write a program tokenring that gets 3 integers - n, p and t - from the command line. When executed, it creates n processes connected to each other by “named pipes”. The named pipes are designated pipe1to2, pipe2to3,..., pipento1 and each allows one-way communication between the i-th and the i+1-th processes. The last named pipe closes the ring allowing process n to communicate with process 1. Once this process ring is created, p1 should send a token (a message with an integer with an initial value 0) to the next process (p1 > p2) and so on (p2 > p3 >... > pn > p1 >...). The token must circulate between processes endlessly, increasing its value at each hop. Each time a process receives the token, it must immediately resend it to the next process or, with a probability of p, block its submission during t seconds and print a message alerting to this fact (see the following example). In either case, the value of the token must be incremented
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char* argv[]){
    
    // Check if the user has entered the correct number of arguments
    if (argc != 4) {
        printf("correct usage: samples filename n_positions m_characters\n");
        return EXIT_FAILURE;
    }

    int token = 0;
    int fork_counter = 0;
    int process_number = 1;
    char process_number_char[1];
    pid_t pid;

    while(true){
        //Creates n processes
        if(fork_counter < atoi(argv[1])){
            pid = fork();
            if (pid < 0) {
                //Error forking
                perror("fork failed");
                return EXIT_FAILURE;
            }
            fork_counter++;
        }

    }
}