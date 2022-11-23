//Write a program txt2epub that given a list of n text files - f1.txt, f2.txt,..., fn.txt - on the command line, applies the pandoc command to each of the files generating EPUB versions The conversion from text to EPUB must be done in parallel by n processes created for this purpose. Each process gets a name from argv[] and converts it as described. After the processes have finished converting all the files, the last step of the program txt2epub will be the generation of a .zip file with the n files in EPUB format
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int i;
    pid_t pid;
    char*  lst_files[argc+1];
    lst_files[0] = "zip";
    lst_files[1] = "ebooks.zip";
    char* file_name_with_epub;
    for(i=1; i<argc; i++){
        file_name_with_epub = strcat(strncpy(malloc(strlen(argv[i])+5), argv[i], strlen(argv[i])-4), ".epub");
        lst_files[i+1] = file_name_with_epub;
        pid = fork();
        if(pid == 0){
            printf("Child %d: %s, %s\n", i, file_name_with_epub, argv[i]);
            execlp("pandoc", "pandoc", argv[i], "-o", file_name_with_epub, NULL);
        }
        wait(NULL);
    }
    execvp("zip", lst_files);
    return 0;
}