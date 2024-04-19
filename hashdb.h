// locks have NOT been implemented yet.
// this thing is like super fucked up
// feel free to fix anything in the meantime, if not i'll just
// come back to it when i'm less stupid tomorrow

// this file has said "less stupid tomorrow" for 2 days now.
// will the day i become less stupid ever come?

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

    ret->lock = (rwlock_t *) malloc(sizeof(rwlock_t));
    rwlock_init(ret->lock);
}

hashRecord * create_record(char *key, uint32_t value, uint32_t hash) {
    hashRecord *ret = (hashRecord *) malloc(sizeof(hashRecord));
    strcpy(ret->name, key);
    ret->salary = value;
    ret->hash = hash;
    ret->next = NULL;
    return ret;
}

// NOTE: char *key is const uint8_t in wikipedia hash func. if issues
// arise, maybe change back, but hey. what could happpen. ahfhfhgh
void insert(char *key, int value, list *table) {
    uint32_t hash = jenkins_hash(key, strlen(key));
    // TODO: write writer lock acquisition

    // this is so convoluted i'm so sorry
    hashRecord *cur = table->head;
    hashRecord *tmp;
    hashRecord *prev;

    // store the pointer to avoid searching twice.
    // the thing i use tmp for varies which is confusing but i'm tired
    tmp = search(key, table);
    if(tmp != NULL) {
        // go straight to the record and update it... i guess
        tmp->salary = value;
    }
    // hash not in table
    else { 
        if(table->head == NULL) {
            // empty case
            tmp = create_record(key, value, hash);
            table->head = tmp;
        }
        // one node case
        else if (table->head->next == NULL) {
            tmp = create_record(key, value, hash);

            if(table->head->hash < hash) {
                // tmp has a smaller hash value, insert at front
                tmp->next = table->head;
                table->head = tmp;
            }
            // tmp larger, insert after head
            else table->head->next = tmp;
        }
        else {
            // i'm doing it like this since it has to be in sorted order..
            // this is fucked up but i'm basically trying to find where to
            // insert the node i guess
            while(cur->next != NULL && cur->hash <= hash)
                cur = cur->next;
            tmp = create_record(key, value, hash);

            // our hash is smaller than head, i think...
            if(cur == table->head) {
                // is this logic even correct??ugh
                tmp->next = table->head;
                table->head = tmp;
            }
            else {
                // a) the nice case. we are at the end of the list.
                if(cur->next == NULL) cur->next = tmp;

                // b) i am but god's little jester, powerless before the 
                //    cop4600 sandwich.
                else {
                    // i may be making a sloppy mistake here but i'm basically
                    // trying to disassemble the list and then reattach it
                    // with the new node sandwiched in the middle
                    prev = cur;
                    cur = cur->next;
                    prev->next = tmp;
                    tmp->next = cur;
                }
            }
        }
    }
    // TODO: release write lock
    return;
}

void delete(char *key, list *table) {
    uint32_t hash = jenkins_hash(key, strlen(key));
    // TODO: writer lock acquisition
    hashRecord *tmp = search(key, table);
    hashRecord *cur = table->head;
    if(tmp != NULL) {
        if(tmp->next == NULL) {
            // tmp is only list item
            table->head = NULL;
            free(tmp);
        }
        else {
            // this is so inefficient but today is not the day i
            // become smart either
            while (cur->next != NULL && cur->next->hash != hash) {
                // should already be sorted by hash or so i hope :(
                if(cur->hash < hash) cur = cur->next;
            }
            // connecting the 2 nodes around tmp
            cur->next = tmp->next;
            free(tmp);
        }
    }
    // TODO: release write lock
    return;
}

hashRecord * search(char *key, list *table) {
    if(table->head == NULL) return NULL;

    // step 1: compute hash value
    uint32_t hash = jenkins_hash(key, strlen(key));

    // TODO: reader lock acquisition

    // iterate through list & compare hash
    hashRecord *cur = table->head;
    while(cur->next != NULL) {
        if(cur->hash != hash) {
            cur = cur->next;
        }
        else {
            // print hash, name, and salary of the record, then return
            printf("%d,%s,%d\n", cur->hash, cur->name, cur->salary);
            //TODO: lock release
            
            return cur;
        }
    }
    printf("No Record Found\n");
    // TODO: lock release
    return NULL;
}
