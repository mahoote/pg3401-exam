#ifndef __THREADS_H__
#define __THREADS_H__

#define BUF_SIZE 4096

typedef struct
{
    unsigned char uszBuffer[BUF_SIZE];
    int iBufferBytes;
    int iByteCounts[256];
    pthread_mutex_t tMutex;
    pthread_cond_t tDataAvailable;
    pthread_cond_t tDataProcessed;
} THREAD_DATA;

void *threadFuncA(void *arg);
void *threadFuncB(void *arg);

#endif