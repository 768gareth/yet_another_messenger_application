#ifndef NETWORKING_H
#define NETWORKING_H

// Using port 10242 for communication between hosts. Unlikely to overlap with any other services.
#define MAIN_PORT 10242

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int send_message_get_connection(const char* v4_address);
int send_message_to_socket(int socket_descriptor, const char* message);

#endif