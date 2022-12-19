#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "include/threads.h"

void *threadFuncA(void *arg)
{
    // Open the PDF file
    FILE *fPDF = fopen("../PG3401-Hjemmeeksamen-14dager-H22.pdf", "rb");
    if (fPDF == NULL)
    {
        perror("Error opening PDF file");
        return NULL;
    }

    THREAD_DATA *data = (THREAD_DATA *)arg;

    while (1)
    {
        // Read bytes from the PDF file into the uszBuffer
        data->iBufferBytes = fread(data->uszBuffer, 1, BUF_SIZE, fPDF);

        // Check if we've reached the end of the file
        if (data->iBufferBytes == 0)
        {
            break;
        }

        // Signal Thread B that data is available
        pthread_mutex_lock(&data->tMutex);
        pthread_cond_signal(&data->tDataAvailable);
        pthread_mutex_unlock(&data->tMutex);

        // Wait for Thread B to signal that the data has been processed
        pthread_mutex_lock(&data->tMutex);
        pthread_cond_wait(&data->tDataProcessed, &data->tMutex);
        pthread_mutex_unlock(&data->tMutex);
    }

    // Signal Thread B that we've reached the end of the file
    pthread_mutex_lock(&data->tMutex);
    pthread_cond_signal(&data->tDataAvailable);
    pthread_mutex_unlock(&data->tMutex);

    fclose(fPDF);

    return NULL;
}

void *threadFuncB(void *arg)
{
    int iByteCounts[256];
    memset(iByteCounts, 0, sizeof(iByteCounts));

    THREAD_DATA *tData = (THREAD_DATA *)arg;

    while (1)
    {
        // Wait for tData to be available from Thread A
        pthread_mutex_lock(&tData->tMutex);
        pthread_cond_wait(&tData->tDataAvailable, &tData->tMutex);
        pthread_mutex_unlock(&tData->tMutex);

        // Check if we've reached the end of the file
        if (tData->iBufferBytes == 0)
        {
            break;
        }

        // Count the number of occurrences of each byte value in the uszBuffer
        for (int i = 0; i < tData->iBufferBytes; i++)
        {
            iByteCounts[tData->uszBuffer[i]]++;
        }

        // Signal Thread A that the tData has been processed
        pthread_mutex_lock(&tData->tMutex);
        pthread_cond_signal(&tData->tDataProcessed);
        pthread_mutex_unlock(&tData->tMutex);
    }

    // Print the number of occurrences of each byte value
    for (int i = 0; i < 256; i++)
    {
        printf("Byte value %02X: %d occurrences\n", i, iByteCounts[i]);
    }

    return NULL;
}