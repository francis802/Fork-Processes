#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    // Check if the user has entered the correct number of arguments
    if (argc != 4) {
        printf("correct usage: samples filename n_positions m_characters\n");
        return EXIT_FAILURE;
    }

    FILE* file;
    file = fopen(argv[1],"r");

    // Check if the file exists
    if (file == NULL) {
        printf("error: cannot open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[2]);
    int m = atoi(argv[3]);

    //Counts the number of characters in the file
    int count = 0;
    char ch;
    for (ch = getc(file); ch != EOF; ch = getc(file))
        count = count + 1;

    //This substraction is to account for the possibility of the text section being in the last m positions of the file
    count-=m;
    
    srand(0);

    //Prints the random text sections of length m from the file
    int random;
    int j; char c;
    for(int i=0; i<n; i++) {
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
