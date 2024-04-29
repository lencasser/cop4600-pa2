
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hashdb.h"
#include "rwlocks.h"

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
  

    ret->lock = (rwlock_t *) malloc(sizeof(rwlock_t));
    rwlock_init(ret->lock);
    return ret;
}

void print(list *table, FILE *out) {
    hashRecord *cur = table->head;
    while (cur) {
       printf("%u,%s,%d\n", cur->hash, cur->name, cur->salary);
       fprintf(out, "%u,%s,%d\n", cur->hash, cur->name, cur->salary);
       cur = cur->next;
    }
}

hashRecord * create_record(char *key, uint32_t value, uint32_t hash) {
    hashRecord *ret = (hashRecord *) malloc(sizeof(hashRecord));
    strcpy(ret->name, key);
    ret->salary = value;
    ret->hash = hash;
    ret->next = NULL;
    return ret;
}

void insert(char *key, int value, list *table,FILE *out) {
    uint32_t hash = jenkins_hash(key, strlen(key));

    hashRecord *cur = table->head;
    hashRecord *tmp;
    hashRecord *prev;

    tmp = search(key, table, out);
    if(tmp != NULL) {
        // go straight to the record and update it
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


                else {
  
                    prev = cur;
                    cur = cur->next;
                    prev->next = tmp;
                    tmp->next = cur;
                }
            }
        }
    }
    
    return;
}

void delete(char *key, list *table,FILE* out) {
    uint32_t hash = jenkins_hash(key, strlen(key));
    hashRecord *tmp = search(key, table,out);
    hashRecord *cur = table->head;
    if(tmp != NULL) {
        if(tmp->next == NULL) {
            // tmp is only list item
            table->head = NULL;
            free(tmp);
        }
        else if(temp == table->head) { //tmp is at the head of the table
            table->head = tmp->next; 
            free(tmp);
        }
        else {
            while (cur != NULL && cur->next != tmp) {
                cur = cur->next;
            }
            if(cur != NULL) {
                cur->next = tmp->next;
                free(tmp);
            }
        }
    }
    return;
}

hashRecord * search(char *key, list *table, FILE *out) {
    if(table->head == NULL) return NULL;

    // step 1: compute hash value
    uint32_t hash = jenkins_hash(key, strlen(key));

    // iterate through list & compare hash
    hashRecord *cur = table->head;
    while(cur->next != NULL) {
        if(cur->hash != hash) {
            cur = cur->next;
        }
        else {
            // print hash, name, and salary of the record, then return
            printf("%u,%s,%d\n", cur->hash, cur->name, cur->salary);
            fprintf(out, "%u,%s,%d\n", cur->hash, cur->name, cur->salary);
            return cur;
        }
    }
    return NULL;
}
