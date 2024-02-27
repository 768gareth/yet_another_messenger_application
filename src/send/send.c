#include "send.h"

int send_message_get_connection(const char* v4_address);
int send_message_to_socket(int socket_descriptor, const char* message);

// Connect to the target address V4_ADDRESS. Return the socket descriptor, or -1 for errors.
int send_message_get_connection(const char* v4_address)
{
    // Acquire a socket descriptor.
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    printf("Using socket %d for sending message...\n", socket_descriptor);

    struct sockaddr_in target_socket;
    target_socket.sin_addr.s_addr = inet_addr(v4_address);
    target_socket.sin_family = AF_INET;
    target_socket.sin_port = htons(MAIN_PORT);

    printf("Connecting to target...\n");

    int connection_status = connect(socket_descriptor, (struct sockaddr*)&target_socket, sizeof(target_socket));

    if (connection_status == -1)
    {
        perror("Connect() returned -1.\n");
        return -1;
    }

    return socket_descriptor;
}

// Send MESSAGE to SOCKET_DESCRIPTOR. Returns 0 for success or -1 for errors.
int send_message_to_socket(int socket_descriptor, const char* message)
{
    size_t length_of_message = strlen(message);

    printf("Sending message...\n");

    ssize_t number_of_bytes_sent = send(socket_descriptor, message, length_of_message, 0);

    printf("Number of bytes sent: %d.\n", number_of_bytes_sent);
    
    if (number_of_bytes_sent == -1)
    {
        perror("Number of bytes sent == -1.\n");
        close(socket_descriptor);
        return -1;
    }

    if (number_of_bytes_sent != length_of_message)
    {
        perror("Number of bytes sent != length of message.");
        close(socket_descriptor);
        return -1;
    }

    close(socket_descriptor);

    return 0;
}