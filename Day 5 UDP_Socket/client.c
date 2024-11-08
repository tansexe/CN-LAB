#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define PORT 12345
#define BUFFER_SIZE 1024
 
int main() {
int sockfd;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE];
 
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
 
printf("Enter message to send to server: ");
fgets(buffer, BUFFER_SIZE, stdin);
 
// Send message to server
if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
perror("sendto failed");
close(sockfd);
exit(EXIT_FAILURE);
}

printf("Message sent to server: %s", buffer);
 
close(sockfd);
return 0;
}
