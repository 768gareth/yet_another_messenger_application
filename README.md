# Yet Another Messenger Application
## Project Description
'Yet Another Messenger Application' is a simple P2P messenger application written in C, for Ubuntu 23.10. Version 1.0 allows for sending of messages between hosts, using a GTK 3.0 GUI.

## Requirements
* Ubuntu 23.10 operating system.
* GTK 3.0 (including dev libraries if compiling from source).
* C 2017.

## Usage
1. Ensure that you have all dependencies installed. This won't work if you don't have them installed.
   
2. Compile using make && make clean, or download latest release.
   
3. Run binary executable via terminal (e.g './yet_another_messenger_application').
   
4. Enter a target address and message content, then click 'Send message' to send the message to the target.
   
5. If the target is running this application, and the connection is successfully established, you will have successfully sent a message to a remote host.

## Design
### Project Structure
1. Graphics - Initialisation and management of GTK GUI, including passing messages to the user interface, handling user input, and retrieving messages from the message queue system.
2. Send - Connecting and sending messages to the remote host target using given user input.
3. Receive - Listening for and receiving messages from remote hosts, and adding received messages to a queue system for retrieval and adding to the UI.
4. Utils - Getting and returning the current UTC time as a string.

## GUI Design
The GUI consists of a single window, split into two sections. 

The top section contains a scrollable GtkTreeView object, in which sent and received messages are displayed. This includes the timestamp, sender, receiver, and content of each sent or received message.

The bottom section contains two input fields, and two buttons. The input fields allow the user to input the IPv4 address of their target, and a message to send. The buttons allow the message to be sent, and for the GtkTreeView to be cleared of all messages, respectively.
