#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    
    // Check if the user has entered the correct number of arguments
    if (argc < 2) {
        printf("correct usage: text2epu filename_1 filename_2 ... filename_n\n");
        return EXIT_FAILURE;
    }

    int i;
    pid_t pid;

    //Create array to store all commands needed to zip the ebooks
    char*  lst_files[argc+1];
    lst_files[0] = "zip";
    lst_files[1] = "ebooks.zip";

    char* file_name_with_epub;

    for(i=1; i<argc; i++){
        //Substitue the .txt extension in file for .epub and save it in array
        file_name_with_epub = strcat(strncpy(malloc(strlen(argv[i])+5), argv[i], strlen(argv[i])-4), ".epub");
        lst_files[i+1] = file_name_with_epub;

        pid = fork();
        if (pid < 0) {
            //Error forking
            perror("fork failed");
            return EXIT_FAILURE;
        }

        if (pid != 0) printf("[pid%d] converting %s ...\n", pid, argv[i]);
        else {
            execlp("pandoc", "pandoc", argv[i], "-o", file_name_with_epub, NULL);
        }
        waitpid(pid, NULL, 0);//Wait for child procedure to die
    }
    lst_files[argc+1] = NULL; //Last element of array must be NULL for execvp to operate well
    execvp("zip", lst_files);
    return 0;
}