#ifndef RWLOCK_H_INCLUDE
#define RWLOCK_H_INCLUDE

#include <semaphore.h>

typedef struct _rwlock_t {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

//creats a lock
void rwlock_init(rwlock_t *lock);


//read locks do not block other read locks. Multiple readers can work at the same time
//read locks only block writers. Writer locks block everyone

//acquires the lock for a reader
void rwlock_acquire_readlock(rwlock_t *lock);
//releases the lock for a reader
void rwlock_release_readlock(rwlock_t *lock);

//acquires the lock for a writer
void rwlock_acquire_writelock(rwlock_t *lock);
//releases the lock for a writer
void rwlock_release_writelock(rwlock_t *lock);

#endif