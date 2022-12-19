#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "include/threads.h"

/* main() -------------------------------------------
    Revision    : 1.0.0

    Comments:
    Initializes a mutex and two condition variables to sync
    the threads. Creates two threads and waits for them to finish.
    Cleans up before exiting program.
   -------------------------------------------------- */
int main()
{
    THREAD_DATA tData;
    pthread_t threadA, threadB;

    pthread_mutex_init(&tData.tMutex, NULL);
    pthread_cond_init(&tData.tDataAvailable, NULL);
    pthread_cond_init(&tData.tDataProcessed, NULL);

    if (pthread_create(&threadA, NULL, threadFuncA, &tData) != 0)
    {
        perror("Error creating Thread A");
        return 1;
    }
    if (pthread_create(&threadB, NULL, threadFuncB, &tData) != 0)
    {
        perror("Error creating Thread B");
        return 1;
    }

    // Wait for Thread A and Thread B to complete
    if (pthread_join(threadA, NULL) != 0)
    {
        perror("Error waiting for Thread A to complete");
        return 1;
    }
    if (pthread_join(threadB, NULL) != 0)
    {
        perror("Error waiting for Thread B to complete");
        return 1;
    }

    pthread_mutex_destroy(&tData.tMutex);
    pthread_cond_destroy(&tData.tDataAvailable);
    pthread_cond_destroy(&tData.tDataProcessed);
    return 0;
}