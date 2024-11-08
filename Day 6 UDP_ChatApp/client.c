#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int sockfd;
struct sockaddr_in server_addr;
socklen_t addr_len = sizeof(server_addr);

// Function to receive messages
void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    ssize_t recv_len;

    while (1) {
        recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                            (struct sockaddr *)&server_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[recv_len] = '\0';
        printf("Server: %s\n", buffer);
    }
    return NULL;
}

int main() {
    char buffer[BUFFER_SIZE];
    pthread_t recv_thread;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with server IP if different
    server_addr.sin_port = htons(PORT);

    // Send initial message to establish connection
    sendto(sockfd, "Hello Server", strlen("Hello Server"), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Create a thread to receive messages
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // Send messages to server
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);
    return 0;
}

