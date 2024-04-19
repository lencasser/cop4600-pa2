// chash.c file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "hashdb.h"
#include "hashdb.c"

void print() {
    
}

void finalPrint() {
    printf("Number of lock acquisitions: %d"); //TODO count number of lock acquisitions and releases
    printf("Number of lock releases: %d");
    print();
}

// so sorry if adding the list *table ptrs to everything is wrong
void parseCommand(char* currCommand, char* currParameter1, char* currParameter2, list *table) {
    print("%s", currCommand);
    if (strcmp(currCommand,"threads")) {
        return;
    }
    else if (strcmp(currCommand,"insert")) {
        int value = atoi(currParameter1);
        
        // srra note: i know the TODO said add hash, but you don't
        // actually need the hash as a parameter since aedo said
        // the insert function computes the hash first

        // the passing in of table and stuff might change depending on
        // how the lock stuff is set up (?) and like. um. idk   . ah
        insert(currParameter1, atoi(currParameter2), table);
    }
    else if (strcmp(currCommand,"print")) {
        //TODO: Get read lock
        print();
        //TODO: Release read lock
    }
    else if (strcmp(currCommand,"search")) {
        // again i might be missing something huge here and maybe
        // we can remove table as a parameter after all. for now,
        // edited in accordance with hashdb.c function structure
        search(currParameter1, table);
    }
    else if (strcmp(currCommand,"delete")) {
        delete(currParameter1, table);
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

    // creating hash table in advance wee
    list *table = create_list();

    // Read the content and print it
    currChar = getchar();
    while(currChar != EOF) { //If we're not at the end of the file yet
        
        while (currChar != '\n') { //For every line

            while (currChar != ',') { //Get the command
                currCommand[currentWordIdx] = currChar;
                currentWordIdx++;
                currChar = getchar();
            }

            currChar = getchar(); //Move currChar from ',' to the next letter
            currentWordIdx = 0; //Reset the index

            while (currChar != ',') { //Get the first parameter
                currParameter1[currentWordIdx] = currChar;
                currentWordIdx++;
                currChar = getchar();
            }

            currChar = getchar(); //Move currChar from ',' to the next letter
            currentWordIdx = 0; //Reset the index

            while (currChar != ',') { //Get the second parameter
                currParameter2[currentWordIdx] = currChar;
                currentWordIdx++;
                currChar = getchar();
            }

            parseCommand(currCommand,currParameter1,currParameter2, table);
            
        }
        //printf("%s", inputString);
    }

    fclose(in);
    return 0;
}
