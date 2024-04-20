#include <stdio.h>

#include "rwlocks.h"

int lock_acquisitions;
int lock_releases;

void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;
    sem_init(&lock->lock, 0, 1);
    sem_init(&lock->writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *lock) {
    sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
        sem_wait(&lock->writelock);
    lock_acquisitions++;
    sem_post(&lock->lock);
    printf("READ LOCK ACQUIRED\n");
}

void rwlock_release_readlock(rwlock_t *lock) {
    sem_wait(&lock->lock);
    lock->readers--;
    lock_releases++;
    if (lock->readers == 0)
	    sem_post(&lock->writelock);
    sem_post(&lock->lock);
    printf("READ LOCK RELEASED\n");
}

void rwlock_acquire_writelock(rwlock_t *lock) {
    sem_wait(&lock->writelock);
    lock_acquisitions++;
    printf("WRITE LOCK ACQUIRED\n");
}

void rwlock_release_writelock(rwlock_t *lock) {
    lock_releases++;
    sem_post(&lock->writelock);
    printf("WRITE LOCK RELEASED\n");
}
