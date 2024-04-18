// chash.c file
#include <stdio.h>;
#include <stdlib.h>
#include <string.h>;


#include "hashdb.h";
#include "hashdb.c";

void print() {
    
}

void finalPrint() {
    printf("Number of lock acquisitions: %d"); //TODO count number of lock acquisitions and releases
    printf("Number of lock releases: %d");
    print();
}



void parseCommand(char* currCommand, char* currParameter1, char* currParameter2) {
    print("%s", currCommand);
    if (strcmp(currCommand,"threads")) {
        return;
    }
    else if (strcmp(currCommand,"insert")) {
        int value = atoi(currParameter1);
        insert(currParameter1,currParameter2,"TODO: Put hash here");
    }
    else if (strcmp(currCommand,"print")) {
        //TODO: Get read lock
        print();
        //TODO: Release read lock
    }
    else if (strcmp(currCommand,"search")) {
        search(currParameter1,"TODO: Put hash here");
    }
    else if (strcmp(currCommand,"delete")) {
        delete(currParameter1);
    }
    else {
        printf("Command %s not found", currCommand);
    }

    return;
}




int main (void) {

    FILE *in;

    in = fopen("commands.txt", "r");

    char currentWord[100];
    char currChar;
    int currentWordIdx = 0;
    int state = 0;
    char currCommand[100];
    char currParameter1[100];
    char currParameter2[200];

    // Read the content and print it
    currChar = getchar();
    while(currChar != EOF) { //If we're not at the end of the file yet
        
        while (currChar != '\n') { //For every line

            while (currChar != ',') { //Get the command
                currCommand[currentWordIdx] = currChar;
                currentWordIdx++;
                currChar = getchar();
            }

            currChar = getChar(); //Move currChar from ',' to the next letter
            currentWordIdx = 0; //Reset the index

            while (currChar != ',') { //Get the first parameter
                currParameter1[currentWordIdx] = currChar;
                currentWordIdx++;
                currChar = getchar();
            }

            currChar = getChar(); //Move currChar from ',' to the next letter
            currentWordIdx = 0; //Reset the index

            while (currChar != ',') { //Get the second parameter
                currParameter2[currentWordIdx] = currChar;
                currentWordIdx++;
                currChar = getchar();
            }

            parseCommand(currCommand,currParameter1,currParameter2);
            
        }
        //printf("%s", inputString);
    }

    fclose(in);
    return 0;
}