#ifndef RECEIVE_H
#define RECEIVE_H

#define MESSAGE_QUEUE_MAX_MESSAGES 20
#define MAIN_PORT 10242

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include "../graphics/graphics.h"

struct message
{
    char* source_address;
    char* message_content;
};

extern struct message message_queue[MESSAGE_QUEUE_MAX_MESSAGES];

void receive_master();
void initialise_message_queue();
void clear_message_queue();

#endif