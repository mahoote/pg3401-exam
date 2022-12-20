#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/contentType.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int iSockFd;
    unsigned short ushPort;
    struct sockaddr_in srvAddr = {0};
    char *pszHostAddress = "127.0.0.1";

    ushPort = atoi("8080");

    // Create a socket
    iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (iSockFd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Bind the socket
    memset(&srvAddr, 0, sizeof(srvAddr));

    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(ushPort);
    srvAddr.sin_addr.s_addr = inet_addr(pszHostAddress);

    if (bind(iSockFd, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) < 0)
    {
        perror("Error binding socket");
        exit(1);
    }

    // Print hyperlink to server.
    printf("Server startet at \033]8;;http://%s:%d\033\\http://%s:%d\033]8;;\033\\\n",
           pszHostAddress, ushPort, pszHostAddress, ushPort);

    while (1)
    {
        // Listen for incoming connections
        listen(iSockFd, 5);

        // Accept incoming connections
        struct sockaddr_in cliAddr;
        socklen_t uiCliAddrSize = sizeof(cliAddr);

        int iClientAccept = accept(iSockFd, (struct sockaddr *)&cliAddr, &uiCliAddrSize);
        if (iClientAccept < 0)
        {
            perror("Error accepting connection");
            exit(1);
        }

        // Read the incoming GET request
        char szBuffer[BUF_SIZE];
        int iRecvStatus = recv(iClientAccept, szBuffer, BUF_SIZE - 1, 0);

        if (iRecvStatus < 0)
        {
            perror("Error reading from socket");
            exit(1);
        }
        szBuffer[iRecvStatus] = '\0';

        // Parse the GET request to extract the file path
        char *pszFilePath = NULL;
        if (strncmp(szBuffer, "GET ", 4) == 0)
        {
            // Check if the path starts with a '/' and remove it.
            pszFilePath = szBuffer + 4;
            if (pszFilePath[0] == '/')
            {
                pszFilePath++;
            }

            char *pszEndOfPath = strchr(pszFilePath, ' ');
            if (pszEndOfPath != NULL)
            {
                *pszEndOfPath = '\0';
            }

            // Redirect to index page if path is empty.
            if (strcmp(pszFilePath, "") == 0)
            {
                memset(pszFilePath, '\0', sizeof(pszFilePath));
                strcpy(pszFilePath, "public/index.html");
            }
        }

        printf("GET /%s\n", pszFilePath);

        // Open the requested file
        FILE *pFile = fopen(pszFilePath, "r");
        if (pFile == NULL)
        {
            perror("Error opening file");
        }
        else
        {
            // Set correct Content-Type.
            char *pszContentType = getContentType(pszFilePath);

            // Send the header to the client
            char szHeader[256];
            sprintf(szHeader, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", pszContentType);
            send(iClientAccept, szHeader, strlen(szHeader), 0);

            // Send the contents of the file
            char szDataBuffer[BUF_SIZE];
            size_t iNumBytesRead;
            while ((iNumBytesRead = fread(szDataBuffer, 1, BUF_SIZE, pFile)) > 0)
            {
                send(iClientAccept, szDataBuffer, iNumBytesRead, 0);
            }

            // Close the file
            fclose(pFile);
        }

        // Close the client socket
        close(iClientAccept);
    }

    // Close the server socket
    close(iSockFd);

    return 0;
}