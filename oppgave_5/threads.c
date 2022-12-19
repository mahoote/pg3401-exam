#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "include/threads.h"

/* threadFuncA() ------------------------------------
    Revision    : 1.0.0

    Comments:
    Reads bytes from the PDF file, stores them in the struct
    buffer. Then signals that the data is available. Waits then
    for the data to be processed and repeats this until end of buffer / EOF.
   -------------------------------------------------- */
void *threadFuncA(void *_tData)
{
    FILE *fPDF = fopen("../PG3401-Hjemmeeksamen-14dager-H22.pdf", "rb");
    if (fPDF == NULL)
    {
        perror("Error opening PDF file");
        return NULL;
    }

    THREAD_DATA *tData = (THREAD_DATA *)_tData;

    while (1)
    {
        // Read bytes from the PDF file into the uszBuffer.
        tData->iBufferBytes = fread(tData->uszBuffer, 1, BUF_SIZE, fPDF);

        // EOF.
        if (tData->iBufferBytes == 0)
        {
            break;
        }

        // Signal data is available.
        pthread_mutex_lock(&tData->tMutex);
        pthread_cond_signal(&tData->tDataAvailable);
        pthread_mutex_unlock(&tData->tMutex);

        // Wait for signal that the data has been processed.
        pthread_mutex_lock(&tData->tMutex);
        pthread_cond_wait(&tData->tDataProcessed, &tData->tMutex);
        pthread_mutex_unlock(&tData->tMutex);
    }

    // Signal EOF.
    pthread_mutex_lock(&tData->tMutex);
    pthread_cond_signal(&tData->tDataAvailable);
    pthread_mutex_unlock(&tData->tMutex);

    fclose(fPDF);

    return NULL;
}

/* threadFuncB() ------------------------------------
    Revision    : 1.0.0

    Comments:
    When data is available, it counts the number of
    occurrences of each byte value in the buffer. Then signals
    that the data has been processed. This is repeated until
    the buffer is empty / EOF.
   -------------------------------------------------- */
void *threadFuncB(void *_tData)
{
    int iByteCounts[256];
    memset(iByteCounts, 0, sizeof(iByteCounts));

    THREAD_DATA *tData = (THREAD_DATA *)_tData;

    while (1)
    {
        // Wait for data to be available.
        pthread_mutex_lock(&tData->tMutex);
        pthread_cond_wait(&tData->tDataAvailable, &tData->tMutex);
        pthread_mutex_unlock(&tData->tMutex);

        // EOF.
        if (tData->iBufferBytes == 0)
        {
            break;
        }

        for (int i = 0; i < tData->iBufferBytes; i++)
        {
            iByteCounts[tData->uszBuffer[i]]++;
        }

        // Signal data has been processed.
        pthread_mutex_lock(&tData->tMutex);
        pthread_cond_signal(&tData->tDataProcessed);
        pthread_mutex_unlock(&tData->tMutex);
    }

    int iByteSum = 0;

    // Print.
    for (int i = 0; i < 256; i++)
    {
        printf("Byte value %02X: %d occurrences\n", i, iByteCounts[i]);
        iByteSum += iByteCounts[i];
    }

    printf("\nByte sum: %d\n", iByteSum);

    return NULL;
}