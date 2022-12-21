#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#include "include/client.h"

/* splitString() ------------------------------------
    Revision    : 1.0.0

    Comments:
    Returns a newly allocated string that contains the characters from the original
    string up to the delimiter, or NULL if the delimiter was not found.
    The original string is modified to point to the character immediately after the
    delimiter. If an error occurs, the function prints an error message and exits.
   -------------------------------------------------- */
char *splitString(char **_ppszOriginal, const char *_pcszDelimiter)
{
    char *pszDelimPos = strstr(*_ppszOriginal, _pcszDelimiter);
    if (pszDelimPos == NULL)
    {
        return NULL;
    }

    size_t uiLength = pszDelimPos - *_ppszOriginal;
    char *pszNewString = malloc(uiLength + 1);
    if (pszNewString == NULL)
    {
        perror("Error allocating memory for new string");
        exit(1);
    }

    memcpy(pszNewString, *_ppszOriginal, uiLength);
    pszNewString[uiLength] = '\0';
    *_ppszOriginal = pszDelimPos + strlen(_pcszDelimiter);

    return pszNewString;
}

/* main() -------------------------------------------
    Revision    : 1.0.0

    Comments:
    Simple client that sends a HTTP GET request to a specified web server
    and prints the server's response. Must provide URL PATH as arg.
    Connects to the server using a socket. Reads all blocks of data sent
    from the server. Stops reading when everything is received.
   -------------------------------------------------- */
int main(int argc, char *argv[])
{
    int iSockFd;
    unsigned short ushPort;
    struct hostent *pHostnm;
    struct sockaddr_in srvAddr = {0};
    char *pszHostAddress = "127.0.0.1";

    // Check for the correct number of arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: webclient <UrlPath>\n");
        exit(1);
    }

    char *pszUrlPath = argv[1];
    pHostnm = gethostbyname(pszHostAddress);
    ushPort = atoi("8080");

    // Checks for NULL pointer.
    if (pHostnm == (struct hostent *)0)
    {
        fprintf(stderr, "Error getting host name!\n");
        exit(1);
    }

    // Create a socket
    iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (iSockFd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Set up the server address
    memset(&srvAddr, 0, sizeof(srvAddr));

    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(ushPort);
    srvAddr.sin_addr.s_addr = *((unsigned long *)pHostnm->h_addr_list[0]);

    // Connect to the server
    if (connect(iSockFd, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    // Send a GET request to the server
    char request[256];
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", pszUrlPath, pszHostAddress);
    if (send(iSockFd, request, strlen(request), 0) < 0)
    {
        perror("Error sending request");
        exit(1);
    }

    // Create a subfolder to keep all downloaded files.
    struct stat st = {0};
    if (stat("response", &st) == -1)
    {
        printf("Created folder 'response'.");
        mkdir("response", 0700);
    }

    // Wait for the response from the server
    int iRecvStatus;
    int iContentLength = 0;
    char szResponse[BUF_SIZE];
    char *pszHeader = NULL;

    FILE *pResponseFile = fopen("response/response.txt", "w");
    if (pResponseFile == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    do
    {
        // Reading blocks of the response by the size of the buffer.
        // Will stop reading when all the blocks have been sent.
        iRecvStatus = recv(iSockFd, szResponse, BUF_SIZE - 1, 0);

        if (iRecvStatus < 0)
        {
            perror("Error receiving response");
            exit(1);
        }

        // Split the header and body when the delimiter occurs.
        char *pszBody = szResponse;

        if (pszHeader == NULL)
        {
            pszHeader = splitString(&pszBody, "\r\n\r\n");
        }
        if (pszHeader != NULL)
        {
            fprintf(pResponseFile, "%s", pszBody);
        }

        bzero(szResponse, sizeof(szResponse));

    } while (iRecvStatus != 0);

    printf("\n");

    // Get filename from header and rename file to correct format.
    char szFileName[256];

    char *pszHeaderFieldValue = strstr(pszHeader, "File-Name");
    if (pszHeaderFieldValue)
    {
        pszHeaderFieldValue += strlen("File-Name: ");

        while (!isalpha(*pszHeaderFieldValue))
        {
            pszHeaderFieldValue++;
        }

        char *pszEndOfLine = strstr(pszHeaderFieldValue, "\r\n");
        if (pszEndOfLine)
        {
            strncpy(szFileName, pszHeaderFieldValue, pszEndOfLine - pszHeaderFieldValue);
            szFileName[pszEndOfLine - pszHeaderFieldValue] = '\0';
        }
        // If the field is the last in the header.
        else
        {
            strcpy(szFileName, pszHeaderFieldValue);
        }
    }

    char szNewFilePath[256];
    sprintf(szNewFilePath, "response/%s", szFileName);

    if (rename("response/response.txt", szNewFilePath) != 0)
    {
        perror("Error renaming file");
    }

    printf("Saved file 'response/%s'.\n", szFileName);

    free(pszHeader);

    fclose(pResponseFile);
    close(iSockFd);

    return 0;
}