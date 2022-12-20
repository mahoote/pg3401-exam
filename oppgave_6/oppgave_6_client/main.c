#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> // Is required for sockaddr_in

#define BUF_SIZE 1024

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

int main(int argc, char *argv[])
{
    int iSockfd;
    unsigned short ushPort;
    struct hostent *pHostnm;
    struct sockaddr_in saAddr = {0};

    // Check for the correct number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: webclient <url> <port>\n");
        exit(1);
    }

    char *pszUrl = argv[1];

    // The path will be everything after the delimiter.
    char *pszPath = pszUrl;
    char *pszHostName = splitString(&pszPath, "/");

    // Parse the server IP and port from the command line arguments
    pHostnm = gethostbyname(pszHostName);
    ushPort = atoi(argv[2]);

    if (pHostnm == (struct hostent *)0)
    {
        fprintf(stderr, "Error getting host name!\n");
        exit(1);
    }

    // Create a socket
    iSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (iSockfd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Set up the server address
    memset(&saAddr, 0, sizeof(saAddr));
    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(ushPort);
    saAddr.sin_addr.s_addr = *((unsigned long *)pHostnm->h_addr_list[0]);

    // Connect to the server
    if (connect(iSockfd, (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    // Send a GET request to the server
    char request[256];
    sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", pszPath, pszHostName);
    if (send(iSockfd, request, strlen(request), 0) < 0)
    {
        perror("Error sending request");
        exit(1);
    }

    // Wait for the response from the server
    char response[BUF_SIZE];
    memset(response, 0, BUF_SIZE);

    int iRecvStatus;

    do
    {
        // Reading blocks of the response by the size of the buffer.
        // Will stop reading when all the blocks have been sent.
        iRecvStatus = recv(iSockfd, response, BUF_SIZE - 1, 0);

        if (iRecvStatus < 0)
        {
            perror("Error receiving response");
            exit(1);
        }

        // The body will be everything after the delimiter.
        char *body = response;
        char *header = splitString(&body, "\r\n\r\n");

        if (header == NULL)
        {
            printf("%s", response);
        }
        else
        {
            // Print the header and body
            printf("%s", header);
            printf("\n------------------------------\n\n");
            printf("%s", body);

            free(header);
        }

        // printf("%s", szBuffer);
        bzero(response, sizeof(response));
    } while (iRecvStatus != 0);

    printf("\n");

    // Clean up
    close(iSockfd);
    free(pszHostName);

    return 0;
}