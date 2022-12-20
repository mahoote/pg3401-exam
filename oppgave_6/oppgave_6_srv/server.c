#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Bind the socket to an address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8080);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error binding socket");
        exit(1);
    }

    while (1)
    {
        // Listen for incoming connections
        listen(server_socket, 5);

        // Accept incoming connections
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket < 0)
        {
            perror("Error accepting connection");
            exit(1);
        }

        // Read the incoming GET request
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0)
        {
            perror("Error reading from socket");
            exit(1);
        }
        buffer[bytes_received] = '\0';

        // Parse the GET request to extract the file path
        char *file_path = NULL;
        if (strncmp(buffer, "GET ", 4) == 0)
        {
            // Check if the path starts with a '/' and remove it.
            file_path = buffer + 4;
            if (file_path[0] == '/')
            {
                file_path++;
            }

            char *end_of_path = strchr(file_path, ' ');
            if (end_of_path != NULL)
            {
                *end_of_path = '\0';
            }
        }

        printf("%s\n", file_path);

        // Open the requested file
        FILE *file = fopen(file_path, "r");
        if (file == NULL)
        {
            perror("Error opening file");
        }
        else
        {
            // Send the header to the client
            char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
            send(client_socket, header, strlen(header), 0);

            // Send the contents of the file as the response
            char response[BUFFER_SIZE];
            while (fgets(response, BUFFER_SIZE, file) != NULL)
            {
                send(client_socket, response, strlen(response), 0);
            }

            // Close the file
            fclose(file);
        }

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}