// Client side C program to demonstrate Socket
// programming on Windows
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    SOCKET client_fd;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    int result;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize Winsock. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == INVALID_SOCKET)
    {
        printf("Socket creation error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported\n");
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        printf("Connection Failed: %d\n", WSAGetLastError());
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }

    // Send a message
    result = send(client_fd, hello, (int)strlen(hello), 0);
    if (result == SOCKET_ERROR)
    {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }
    printf("Hello message sent\n");

    // Read the server's response
    result = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (result == SOCKET_ERROR)
    {
        printf("Recv failed: %d\n", WSAGetLastError());
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }
    buffer[result] = '\0'; // Null-terminate the buffer
    printf("Server response: %s\n", buffer);

    // Close the socket
    closesocket(client_fd);
    WSACleanup();
    return 0;
}
