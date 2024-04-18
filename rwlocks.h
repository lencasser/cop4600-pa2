// rwlocks.h file
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "common.h"
#include "common_threads.h"

#ifdef linux
#include <semaphore.h>
#elif __APPLE__
#include "zemaphore.h"
#endif

typedef struct _rwlock_t {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;