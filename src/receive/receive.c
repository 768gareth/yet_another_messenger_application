#include "receive.h"
#include "../send/send.h"

struct message message_queue[MESSAGE_QUEUE_MAX_MESSAGES];
int number_of_waiting_messages;
int message_queue_iterator;

void receive_master();
char* receive_message(int receiver_socket);
int add_message_to_queue(char* received_message, char* source_address);
void initialise_message_queue();
void clear_message_queue();

// High-level function that handles the loop of waiting for and receiving messages from other hosts
void receive_master()
{
    // The socket used for listening for new connections.
    int listener_socket = 0;

    // The socket used to receive messages upon.
    int receiving_socket = 0;

    // The string used to store the source address of a received message.
    char* source_address_string = (char*)malloc(INET_ADDRSTRLEN);

    // The string used to store the received message.
    char* received_message_string = (char*)malloc(256 * sizeof(char));

    // Establish listener socket.
    listener_socket = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;

    setsockopt(listener_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Define local address.
    struct sockaddr_in local_address;
    socklen_t local_address_size = sizeof(struct sockaddr_in);
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(MAIN_PORT);
    local_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the local address to the socket. Needed for listening.
    int bind_status = bind(listener_socket, (struct sockaddr*)&local_address, local_address_size);

    // If listener_socket is -1, an error has occurred.
    if (listener_socket == -1)
    {
        perror("Debug: Failed to establish listener socket.\n");
        close(listener_socket);
    }

    // Start listening for new connections.
    listen(listener_socket, SOMAXCONN);

    printf("Started listening for connections on socket %d...\n", listener_socket);

    // Infinite loop, listening for messages.
    while (1)
    {
        // Remote host details, from where the message is being received.
        struct sockaddr_in source_address;
        socklen_t source_address_size = sizeof(struct sockaddr_in);

        // accept() will wait until a new connection has been established, then proceed.
        receiving_socket = accept(listener_socket, (struct sockaddr*)&source_address, &source_address_size);

        printf("Accepted new connection on socket %d...\n", receiving_socket);

        // If accept() has returned -1, this indicates a failure to accept the new connection.
        if (receiving_socket == -1)
        {
            perror("Debug: Failed to accept new connection via listener socket.\n");
            close(receiving_socket);
            continue;
        }

        // Get the IP address from the source address.
        source_address_string = inet_ntoa(source_address.sin_addr);

        // Receive the message on the receiving socket.
        received_message_string = receive_message(receiving_socket);

        close(receiving_socket);

        printf("RECEIVED: %s.\n", received_message_string);

        printf("FROM: %s.\n", source_address_string);

        // If the message is nullptr, the message was not successfully received.
        if (received_message_string == NULL)
        {
            perror("Error: Received null message, indicating a failure to receive a message.\n");
            continue;
        }

        // Add the message to the queue, and perform error checking.
        if (add_message_to_queue(received_message_string, source_address_string) == -1)
        {
            printf("Error: Failed to add received message to the message queue.\n");
            continue;
        }

        // End of loop.
    }
}

// Receive the message on the specified receiver socket, or return NULL to indicate an error.
char* receive_message(int receiver_socket)
{
    // The string in which the message will be received.
    char* received_message_string;

    // Assign memory for the string.
    received_message_string = (char*)malloc(512 * sizeof(char));

    // Number of bytes to be received via recv().
    ssize_t bytes_received = 0;

    // Receive the message. The message is passed to the buffer, and the number of bytes received is given.
    bytes_received = recv(receiver_socket, received_message_string, 1024, 0);

    // If the number of bytes received is -1, an error has occurred.
    if (bytes_received == -1)
    {
        perror("Error: Received -1 bytes.\n");
        return NULL;
    }

    // If the number of bytes received is 0, a different error has occurred.
    if (bytes_received == 0)
    {
        perror("Error: Received 0 bytes.\n");
        return NULL;
    }

    // Add a null terminator at the end of the string.
    received_message_string[bytes_received] = '\0';

    // Return the message.
    return received_message_string;
}

// Add the input message to the queue. Return 0 to indicate success, or -1 for errors.
int add_message_to_queue(char* received_message, char* source_address)
{
    printf("Adding message to queue...\n");

    // If the number of waiting messages is greater than the capacity of the queue's ability to hold them.
    // The message must be discarded to avoid a buffer overflow.
    if (number_of_waiting_messages >= MESSAGE_QUEUE_MAX_MESSAGES)
    {
        printf("Error: Message queue is full. The incoming message has been discarded.\n");
        return -1;
    }

    // Create a new message containing the source address and message.
    struct message new_message_details;

    // Ensure proper initialization of the new_message_details structure.
    memset(&new_message_details, 0, sizeof(struct message));

    new_message_details.source_address = (char*)malloc(strlen(source_address) + 1);

    strncpy(new_message_details.source_address, source_address, strlen(source_address) + 1);

    new_message_details.message_content = (char*)malloc(strlen(received_message) + 1);

    strncpy(new_message_details.message_content, received_message, strlen(received_message) + 1);

    // Add the message, using the message queue iterator value to determine the next available position.
    message_queue[message_queue_iterator] = new_message_details;

    // Increment the value of the next_available position.
    message_queue_iterator++;

    // Return 0 to indicate success.
    return 0;
}

// Assign memory to the message queue.
void initialise_message_queue()
{
    // Set the number of waiting messages to zero.
    number_of_waiting_messages = 0;

    // Set the message queue iterator to zero.
    message_queue_iterator = 0;

    // Assign memory for the maximum number of messages in the queue.
    for (int i = 0; i < MESSAGE_QUEUE_MAX_MESSAGES; i++)
    {
        message_queue[i].source_address = (char*)malloc(sizeof(char) * 16);
        message_queue[i].message_content = (char*)malloc(sizeof(char) * 1024);
        message_queue[i].source_address = NULL;
        message_queue[i].message_content = NULL;
    }
}

// Clear the message queue, this is to be called each time it has been accessed.
void clear_message_queue()
{
    printf("Clearing message queue...\n");

    for (int i = 0; i < MESSAGE_QUEUE_MAX_MESSAGES; i++) 
    {
        if (message_queue[i].message_content != NULL) 
        {
            free(message_queue[i].message_content);
            message_queue[i].message_content = NULL;  // Set to NULL after freeing to avoid double-freeing.
        }

        if (message_queue[i].source_address != NULL) 
        {
            free(message_queue[i].source_address);
            message_queue[i].source_address = NULL;  // Set to NULL after freeing to avoid double-freeing.
        }
    }

    // Set the number of waiting messages to zero.
    number_of_waiting_messages = 0;

    // Set the message queue iterator to zero.
    message_queue_iterator = 0;

    return;
}