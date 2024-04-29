// chash.c file
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "rwlocks.h"
//#include "rwlocks.c"
#include "hashdb.h"
#include "hashdb.c"

FILE* out;

void finalPrint(list *table, FILE *out) {
    printf("Number of lock acquisitions: %d\n", lock_acquisitions);
    printf("Number of lock releases: %d\n", lock_releases);
    fprintf(out, "Number of lock acquisitions: %d\n", lock_acquisitions);
    fprintf(out, "Number of lock releases: %d\n", lock_releases);
    print(table, out);
}

struct parseParams
{
    char currCommand[1000];
    char currParameter1[1000];
    char currParameter2[1000];
    list *table;
};


void parseCommand(char* currCommand, char* currParameter1, char* currParameter2, list *table, FILE* out) {
    
    //printf("Command: %s\n", currCommand);
    
    if (strcmp(currCommand,"insert")==0) {
        int value = atoi(currParameter1);

        printf("INSERT,%s,%s\n", currParameter1, currParameter2);
        fprintf(out, "INSERT,%s,%s\n", currParameter1, currParameter2);
        rwlock_acquire_writelock(table->lock,out);
        insert(currParameter1, atoi(currParameter2), table, out);
        rwlock_release_writelock(table->lock,out);
    }
    else if (strcmp(currCommand,"print")==0) {
        rwlock_acquire_readlock(table->lock,out);
        print(table,out);
        rwlock_release_readlock(table->lock,out);
    }
    else if (strcmp(currCommand,"search")==0) {
        printf("SEARCH,%s\n", currParameter1);
        fprintf(out, "SEARCH,%s\n", currParameter1);
        rwlock_acquire_readlock(table->lock,out);
        search(currParameter1, table, out);
        rwlock_release_readlock(table->lock,out);
    }
    else if (strcmp(currCommand,"delete")==0) {
        printf("DELETE,%s\n", currParameter1);
        fprintf(out, "DELETE,%s\n", currParameter1);
        rwlock_acquire_writelock(table->lock,out);
        delete(currParameter1, table, out);
        rwlock_release_writelock(table->lock,out);
    }
    else {
        printf("Command %s not found", currCommand);
    }

    return;
}

void *parseCommandThreadWrapper(void *arg)
{
    struct parseParams *p = (struct parseParams *) arg;
    parseCommand(p->currCommand, p->currParameter1, p->currParameter2, p->table, out);
    free(arg);
}

int main (void) {

    FILE *in;
    //FILE* out;

    in = fopen("alldelete.txt", "r"); //Change the text here to change the input file name
    out = fopen("output.txt", "w");

    char currentWord[10000];
    char currChar;
    int currentWordIdx = 0;
    int state = 0;
    char currCommand[1000];
    char currParameter1[1000];
    char currParameter2[1000];
    int i = 0;
    int j = 0;
    pthread_t  *tinfo;
    int thread_index = 0;

    // creating hash table in advance wee
    list *table = create_list();

    // Read the content and print it
    //fscanf(in, "%s", currentWord);
    fgets(currentWord,1000,in);
    //printf("%s",currentWord);

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

    printf("Running %d threads\n",threadCount);
    fprintf(out, "Running %d threads\n", threadCount);

    tinfo = calloc(threadCount, sizeof(pthread_t));
    if (tinfo == NULL) {
        printf("ERROR: calloc failed\n");
        return 0;
    }

    while(currentWord != NULL) { //If we're not at the end of the file yet
        if(!fgets(currentWord,1000,(FILE*)in)) break;

        i=0;
        strcpy(currCommand,"");
        strcpy(currParameter1,"");
        strcpy(currParameter2,"");

        for (i=0;currentWord[i] != ',';i++) {
            currCommand[i] = currentWord[i];
        }
        currCommand[i] = '\0';

        i++; //Move index out of ','

        for (j=0;currentWord[i] != ',';i++) {
            currParameter1[j] = currentWord[i];
            j++;
        }
        currParameter1[j] = '\0';

        i++; //Move index out of ','

        for (j=0;currentWord[i] != '\n';i++) {
            currParameter2[j] = currentWord[i];
            j++;
        }
        currParameter2[j] = '\0';

        if (thread_index > threadCount) {
            printf("no threads left\n");
            exit(0);
        }

        struct parseParams *p = (struct parseParams *)malloc(sizeof(struct parseParams));
        strncpy(p->currCommand, currCommand, sizeof(currCommand));
        strncpy(p->currParameter1, currParameter1, sizeof(currParameter1));
        strncpy(p->currParameter2, currParameter2, sizeof(currParameter2));
        p->table = table;
        pthread_create(&tinfo[thread_index], NULL, parseCommandThreadWrapper, (void *)p);
        thread_index++;
    }

    for(int i=0;i<threadCount;i++)
    {
        void *res;
        pthread_join(tinfo[i], &res);
    }

    finalPrint(table,out);

    fclose(in);
    fclose(out);
    return 0;
}
