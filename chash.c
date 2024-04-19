// chash.c file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "rwlocks.h"
//#include "rwlocks.c"
#include "hashdb.h"
#include "hashdb.c"


void print() {
    
}

void finalPrint() {
    printf("Number of lock acquisitions: "); //TODO count number of lock acquisitions and releases
    printf("Number of lock releases: ");
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

    printf("Starting up\n");

    FILE *in;

    in = fopen("commands.txt", "r");

    char currentWord[10000];
    char currChar;
    int currentWordIdx = 0;
    int state = 0;
    char currCommand[1000];
    char currParameter1[1000];
    char currParameter2[1000];
    int i = 0;
    int j = 0;

    // creating hash table in advance wee
    list *table = create_list();

    // Read the content and print it
    //fscanf(in, "%s", currentWord);
    fgets(currentWord,1000,in);
    printf("%s",currentWord);

    for (i=0;currentWord[i] != ',';i++) {
        currCommand[i] = currentWord[i];
    }

    if (strcmp(currCommand,"threads") != 0) {
        printf("ERROR: First command is not THREAD\nExiting\n");
        return 0;
    }

    i++;
    for (j=0;currentWord[i] != ',';i++) {
        currParameter1[j] = currentWord[i];
        j++;
    }

    int threadCount = atoi(currParameter1);

    printf("Starting %s with count %d/%s\n",currCommand,threadCount,currParameter1);


    while(currentWord != NULL) { //If we're not at the end of the file yet
        if(!fgets(currentWord,1000,(FILE*)in)) break;

        printf("%s\n", currentWord);

        i=0;
        strcpy(currCommand,"");
        strcpy(currParameter1,"");
        strcpy(currParameter2,"");

        //printf("earlyCommand: %s\n", currCommand);

        for (i=0;currentWord[i] != ',';i++) {
            currCommand[i] = currentWord[i];
        }
        currCommand[i] = '\0';

        printf("currComand: %s\n", currCommand);

        i++; //Move index out of ','

        for (j=0;currentWord[i] != ',';i++) {
            currParameter1[j] = currentWord[i];
            j++;
        }
        currParameter1[j] = '\0';

        printf("curr1: %s\n", currParameter1);

        i++; //Move index out of ','

        for (j=0;currentWord[i] != '\n';i++) {
            currParameter2[j] = currentWord[i];
            j++;
        }
        currParameter2[j] = '\0';

        printf("%s%s%s\n", currCommand,currParameter1,currParameter2);

        //parseCommand(currCommand,currParameter1,currParameter2, table);
        
    }


    printf("Done!\n");

    fclose(in);
    return 0;
}
