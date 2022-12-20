#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "include/main.h"

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
    int iSockfd;
    unsigned short ushPort;
    struct hostent *pHostnm;
    struct sockaddr_in saAddr = {0};
    char *pszHostName = "127.0.0.1";

    // Check for the correct number of arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: webclient <UrlPath>\n");
        exit(1);
    }

    char *pszUrlPath = argv[1];

    pHostnm = gethostbyname(pszHostName);
    ushPort = atoi("8080");

    // Checks for NULL pointer.
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
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", pszUrlPath, pszHostName);
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

        // Either print the whole response or split the header and body
        // if the delimiter occurs.
        char *body = response;
        char *header = splitString(&body, "\r\n\r\n");

        if (header == NULL)
        {
            printf("%s", response);
        }
        else
        {
            // Print the header and body
            printf("%s\n\n", header);
            printf("%s", body);

            free(header);
        }

        bzero(response, sizeof(response));

    } while (iRecvStatus != 0);

    printf("\n");

    close(iSockfd);

    return 0;
}