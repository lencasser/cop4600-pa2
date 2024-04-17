// hello team
// this is the version with functions, feel free to just combine and ditch
// my struct if you'd like.
// i did the uint32_t since that was how he defined the struct in the
// assignment directions but if regular old int is better feel free to
// stick with that i don't know what the functional difference would be

#include <stdint.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

// depending on how the lock stuff works might add a list struct?

extern uint32_t jenkins_hash(const uint8_t *key, size_t length);
extern hashRecord * create_record(char *key, uint32_t value, uint32_t hash);
extern void insert(const uint8_t *key, int value, hashRecord *head);
extern void delete(char *key);
extern int search(const uint8_t *key, hashRecord *head);
