// locks have NOT been implemented yet.
// this thing is like super fucked up
// feel free to fix anything in the meantime, if not i'll just
// come back to it when i'm less stupid tomorrow

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hashdb.h"

uint32_t jenkins_hash(char *key, size_t length) {
    size_t i = 0;
    uint32_t hash = 0;
    
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

list * create_list() {
    list *ret = (list *) malloc(sizeof(list));
    ret->head = NULL;
    
    // originally had the below lines bc of the powerpoint lock LL 
    // example but it's not working and some guy in the discord didn't
    // have the lock in their list struct and i trust them with my life

    //ret->lock = (rwlock_t *) malloc(sizeof(rwlock_t));
    //rwlock_init(ret->lock);
}

hashRecord * create_record(char *key, uint32_t value, uint32_t hash) {
    hashRecord *ret = (hashRecord *) malloc(sizeof(hashRecord));
    strcpy(ret->name, key);
    ret->salary = value;
    ret -> hash = hash;
    return ret;
}

// the wikipedia jenkins hash got in my head and i originally had
// the key as const uint8_t. then i changed it to char in accordance
// with the struct. will i change it after i take a nap?
// nobody nose 0>0 (nose) - my condolences

// still have to account for empty list / one node list
void insert(char *key, int value, list *table) {
    uint32_t hash = jenkins_hash(key, strlen(key));
    // TODO: write writer lock acquisition

    hashRecord *cur = table->head;
    hashRecord *tmp;
    hashRecord *prev;

    // tbh i don't think search function is the ideal way to do this,
    // but then why search function.
    if(search(key, table)) {
        while(cur->hash != hash) cur = cur->next;
        cur->salary = value;
    }
    else { 
        if(table->head == NULL) ; // TODO: empty case
        else if (table->head->next == NULL) ; // TODO: one node case
        else {
            // i'm doing it like this since it has to be in sorted order
            // again i am SO sorry if i'm missing something obvious here
            while(cur->hash < hash && cur->next->hash < hash)
                cur = cur->next;
            tmp = create_record(key, value, hash);
            if(cur == table->head) {
                // TODO
            }
            else {
                // pukes and dies
            }
        }
    }
    // TODO: release write lock
    return;
}

void delete(char *key) {
    // TODO
}

uint32_t search(char *key, list *table) {
    uint32_t hash = jenkins_hash(key, strlen(key));
    // TODO: reader lock acquisition
    hashRecord *cur = table->head;

    while(cur->next != NULL) {
        if(cur->hash != hash) {
            cur = cur->next;
        }
        else {
            // TODO: whatever the format of the record printing is
            return hash;
        }
    }
    printf("No Record Found\n");
    // TODO: lock release
    return 0;
}
