#include "graphics.h"
#include "../glade_graphics.h"
#include "../utils/utils.h"
#include "../receive/receive.h"
#include "../send/send.h"

GtkWidget* gtk_window_obj;
GtkBuilder* gtk_builder_obj;

GtkWidget* entry_address;
GtkWidget* entry_message;

GtkWidget* button_send_message;
GtkWidget* button_clear_messages;

GtkTreeView* treeview_messages;
GtkListStore* liststore_messages;

GtkTreeViewColumn* column_timestamp;
GtkTreeViewColumn* column_source;
GtkTreeViewColumn* column_destination;
GtkTreeViewColumn* column_message;

GtkTreeIter* tree_iterator;

void start_interface();
void on_send_button_pressed(void);
void on_clear_button_pressed(void);
gboolean check_for_messages(gpointer user_data);
void add_item_to_text_window(char* message, char* source_address, char* target_address);

// Initialise the graphics interface.
void start_interface()
{
    gtk_init(NULL, NULL);

    gtk_builder_obj = gtk_builder_new();

    gtk_builder_add_from_string(GTK_BUILDER(gtk_builder_obj), messenger_graphics_glade, strlen(messenger_graphics_glade), NULL);

    gtk_window_obj = GTK_WIDGET(gtk_builder_get_object(gtk_builder_obj, "main_window"));

    entry_address = GTK_WIDGET(gtk_builder_get_object(gtk_builder_obj, "gtk_target_entry"));
    entry_message = GTK_WIDGET(gtk_builder_get_object(gtk_builder_obj, "gtk_message_entry"));

    treeview_messages = GTK_TREE_VIEW(gtk_builder_get_object(gtk_builder_obj, "treeview_window"));
    liststore_messages = GTK_LIST_STORE(gtk_builder_get_object(gtk_builder_obj, "liststore_messages"));

    column_timestamp = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(gtk_builder_obj, "column_timestamp"));
    column_source = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(gtk_builder_obj, "column_source"));
    column_destination = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(gtk_builder_obj, "column_destination"));
    column_message = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(gtk_builder_obj, "column_message"));

    button_send_message = GTK_WIDGET(gtk_builder_get_object(gtk_builder_obj, "gtk_button_send_message"));
    button_clear_messages = GTK_WIDGET(gtk_builder_get_object(gtk_builder_obj, "gtk_button_clear_messages"));

    g_signal_connect(gtk_window_obj, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button_send_message, "clicked", G_CALLBACK(on_send_button_pressed), NULL);
    g_signal_connect(button_clear_messages, "clicked", G_CALLBACK(on_clear_button_pressed), NULL);

    gtk_builder_connect_signals(gtk_builder_obj, NULL);
    g_object_unref(gtk_builder_obj);

    g_timeout_add(1000, (GSourceFunc)check_for_messages, NULL);
    
    gtk_widget_show_all(gtk_window_obj);

    gtk_main();
}

void on_send_button_pressed()
{
    printf("Send button pressed...\n");

    char* target_address = (char*) gtk_entry_get_text(GTK_ENTRY(entry_address));
    char* message_to_send = (char*) gtk_entry_get_text(GTK_ENTRY(entry_message));

    printf("Got target address: %s.\n", target_address);
    printf("Got message content: %s.\n", message_to_send);

    // Check that the target is online and has the program running.
    int socket_descriptor = send_message_get_connection(target_address);

    if (socket_descriptor == -1)
    {
        perror("Failed to get socket descriptor.\n");
        add_item_to_text_window("ERROR: Failed to get socket descriptor.", "You", target_address);
    }

    // Send the message.
    int send_message_status = send_message_to_socket(socket_descriptor, message_to_send);

    if (send_message_status == -1)
    {
        perror("Failed to send message.\n");
        add_item_to_text_window("ERROR: Failed to send message.", "You", target_address);
    }

    // Upload the message to the message log.
    add_item_to_text_window(message_to_send, "You", target_address);
    return;
}

// When the 'clear messages' button is pressed.
void on_clear_button_pressed()
{
    gtk_list_store_clear(liststore_messages);
}

// Check through the message log, and add any available messages
gboolean check_for_messages(gpointer user_data)
{
    printf("Checking messages...\n");

    // Iterate through the queue and add all of them.
    for (int i = 0; i < MESSAGE_QUEUE_MAX_MESSAGES; i++)
    {
        if (message_queue[i].message_content != NULL)
        {
            add_item_to_text_window(message_queue[i].message_content, message_queue[i].source_address, "You");
        }
    }

    // Clear the queue.
    clear_message_queue(message_queue);

    return G_SOURCE_CONTINUE;
}
 
// Add an item to the tree, including message, source address, and target address.
void add_item_to_text_window(char* message, char* source_address, char* target_address)
{
    char* current_time = get_current_time();

    printf("Adding source address to tree: %s.\n", source_address);
    printf("Adding target address to tree: %s.\n", target_address);
    printf("Adding message content to tree: %s.\n", message);

    gtk_list_store_append(liststore_messages, &tree_iterator);
    gtk_list_store_set(liststore_messages, &tree_iterator, 0, current_time, 1, source_address, 2, target_address, 3, message, -1);

    free(current_time);
    return;
}