#ifndef _HASHDB_H
#define _HASHDB_H
#include <stdint.h>
#include "rwlocks.h"


typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

typedef struct list {
    hashRecord *head;
    rwlock_t *lock; // ugh
} list;

extern uint32_t jenkins_hash(char *key, size_t length);
extern list * create_list();
extern hashRecord * create_record(char *key, uint32_t value, uint32_t hash);
extern void insert(char *key, int value, list *table);
extern void delete(char *key, list *table);
extern hashRecord * search(char *key, list *table);
extern void print(list *table);

#endif
