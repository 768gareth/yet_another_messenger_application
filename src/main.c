#include "graphics/graphics.h"
#include "receive/receive.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int main()
{
    // Initialise the message queue, ensuring that received messages can be stored successfully.
    initialise_message_queue();

    // Create a listener thread, to listen for and manage messages from other hosts.
    pthread_t listening_thread;

    // Link the new thread to the receiving master function, and perform error checking.
    if (pthread_create(&listening_thread, NULL, receive_master, NULL) != 0) 
    {
        perror("Fatal error: Failed to create listener thread.\n");
        return -1;
    }

    // Initialise the GTK interface.
    start_interface();

    // When the GTK interface closes, kill the listener thread.
    pthread_join(listening_thread, NULL);

    return 0;
}