#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void perform_calculation(int client_socket) {
    char buffer[BUFFER_SIZE];
    int num1, num2;
    char operator;
    int result;

    // Receive the first integer
    read(client_socket, &num1, sizeof(num1));

    // Receive the operator
    read(client_socket, &operator, sizeof(operator));

    // Receive the second integer
    read(client_socket, &num2, sizeof(num2));

    // Perform the calculation based on the operator
    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                strcpy(buffer, "Error: Division by zero\n");
                send(client_socket, buffer, strlen(buffer), 0);
                return;
            }
            break;
        default:
            strcpy(buffer, "Error: Invalid operator\n");
            send(client_socket, buffer, strlen(buffer), 0);
            return;
    }

    // Send the result back to the client
    snprintf(buffer, sizeof(buffer), "Result: %d %c %d = %d\n", num1, operator, num2, result);
    send(client_socket, buffer, strlen(buffer), 0);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept a connection from a client
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected\n");

        // Perform the calculation
        perform_calculation(client_socket);

        // Close the connection
        close(client_socket);
        printf("Client disconnected\n");
    }

    close(server_fd);
    return 0;
}
