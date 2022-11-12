#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {

    /* check if exactly one argument is present */
    if (argc < 4) {
        printf("correct usage: samples filename n_positions m_characters\n");
        return EXIT_FAILURE;
    }

    int n, m;

    FILE* filecount;
    FILE* file;
    file = fopen(argv[1],"r");
    filecount = fopen(argv[1],"r");
    if (file == NULL || filecount == NULL) {
        printf("error: cannot open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    n = atoi(argv[2]);
    m = atoi(argv[3]);



    int count = 0;
    char ch;
    /* Count the amount of characters in the file */
    for (ch = getc(filecount); ch != EOF; ch = getc(filecount))
        // Increment count for this character
        count = count + 1;

    fclose(filecount);
    count-=m;


    srand(0);

    int random;
    int j; char c;
    for(int i=0; i<=n; i++) {
        j = 0;
        random = rand();
        random%=count;

        fseek(file, random, SEEK_SET);
        
        printf(">");
        while(j<m){
            c = fgetc(file);
            printf("%c",c);
            j++;
        }
        printf("<\n");
        
    }
    fclose(file);

}