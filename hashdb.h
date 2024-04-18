#include <stdint.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

typedef struct list {
    hashRecord *head;
    rwlock_t *lock; // this doesn't work i'm prob doing smth dumb
} list;

// depending on how the lock stuff works might add a list struct?

extern uint32_t jenkins_hash(char *key, size_t length);
extern list * create_list();
extern hashRecord * create_record(char *key, uint32_t value, uint32_t hash);
extern void insert(char *key, int value, hashRecord *head);
extern void delete(char *key);
extern uint32_t search(char *key, hashRecord *head);
