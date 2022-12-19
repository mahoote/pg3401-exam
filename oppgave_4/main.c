#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct httpInfo
{
    int iHttpCode;
    int iContentLength;
    float flHttpVersion; // New field.
    bool bSuccess;
    char szServer[16];
    char szContentType[16];
} HTTP_INFO, *PHTTP_INFO;

HTTP_INFO *ProcessHttpHeader(char *pszHttpReply)
{
    char *pszHeaderFieldValue;

    // Fixed
    PHTTP_INFO pHttpInfo = (PHTTP_INFO)malloc(sizeof(HTTP_INFO));

    if (!pHttpInfo)
    {
        return NULL;
    }

    memset(pHttpInfo, 0, sizeof(HTTP_INFO));
    pHttpInfo->iHttpCode = atoi(pszHttpReply + strlen("HTTP/x.x "));

    if (pHttpInfo->iHttpCode == 200)
    {
        pHttpInfo->bSuccess = true;
    }

    // New
    pszHeaderFieldValue = strstr(pszHttpReply, "HTTP/");
    if (pszHeaderFieldValue)
    {
        // Optimized
        pszHeaderFieldValue += strlen("HTTP/");
        char *pszEndOfHttpVersion = strchr(pszHeaderFieldValue, ' ');

        while (!pszEndOfHttpVersion)
        {
            pszHeaderFieldValue++;
        }

        // Fixed
        pHttpInfo->flHttpVersion = atof(pszHeaderFieldValue);
    }

    pszHeaderFieldValue = strstr(pszHttpReply, "Server");
    if (pszHeaderFieldValue)
    {
        // Optimized
        pszHeaderFieldValue += strlen("Server: ");

        while (!isalpha(*pszHeaderFieldValue))
        {
            pszHeaderFieldValue++;
        }

        // Fixed
        char *pszEndOfLine = strstr(pszHeaderFieldValue, "\n");
        if (pszEndOfLine)
        {
            strncpy(pHttpInfo->szServer, pszHeaderFieldValue, pszEndOfLine - pszHeaderFieldValue);
            pHttpInfo->szServer[pszEndOfLine - pszHeaderFieldValue] = '\0';
        }
    }

    pszHeaderFieldValue = strstr(pszHttpReply, "Content-Type");
    if (pszHeaderFieldValue)
    {
        // Optimized
        pszHeaderFieldValue += strlen("Content-Type: ");

        while (!isalpha(*pszHeaderFieldValue))
        {
            pszHeaderFieldValue++;
        }

        // Fixed
        char *pszEndOfLine = strstr(pszHeaderFieldValue, "\n");
        if (pszEndOfLine)
        {
            strncpy(pHttpInfo->szContentType, pszHeaderFieldValue, pszEndOfLine - pszHeaderFieldValue);
            pHttpInfo->szContentType[pszEndOfLine - pszHeaderFieldValue] = '\0';
        }
    }

    pszHeaderFieldValue = strstr(pszHttpReply, "Content-Length");
    if (pszHeaderFieldValue)
    {
        // Optimized
        pszHeaderFieldValue += strlen("Content-Length: ");

        while (!isdigit(*pszHeaderFieldValue))
        {
            pszHeaderFieldValue++;
        }

        // Fixed
        pHttpInfo->iContentLength = atoi(pszHeaderFieldValue);
    }
    return pHttpInfo;
}

int main()
{
    char *pszHttpHeader = "HTTP/1.1 200 OK\nServer: Apache\nContent-Type: text/html\nContent-Length: 1024\n";
    PHTTP_INFO pHttpInfo = ProcessHttpHeader(pszHttpHeader);
    if (!pHttpInfo)
    {
        printf("Error: Failed to process HTTP header\n");
        return 1;
    }
    printf("HTTP code: %d\n", pHttpInfo->iHttpCode);
    printf("HTTP version: %.1f\n", pHttpInfo->flHttpVersion);
    printf("Success: %s\n", pHttpInfo->bSuccess ? "true" : "false");
    printf("Server: %s\n", pHttpInfo->szServer);
    printf("Content type: %s\n", pHttpInfo->szContentType);
    printf("Content length: %d\n", pHttpInfo->iContentLength);

    // Remember to free.
    free(pHttpInfo);
    return 0;
}