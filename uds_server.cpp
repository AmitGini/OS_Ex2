#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include "uds_server.hpp"

using namespace std;

// Create a Unix Domain Socket server using stream
int startUDSServerStream(const string &socketPath)
{
    
    struct sockaddr_un server_addr; // Define the server_addr struct
    int listeningSocket; // Define the listening socket

    /* Create the server socket stream */
    listeningSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(listeningSocket < 0) // check if the socket was created successfully
    {
        perror("socket");
        return -1;
    }

    cout<<"Server Socket Created"<<endl;
    
    /* In case the program exited unexpectedly, the socket file may still exist. */
    unlink(socketPath.c_str()); // remove the socket file if it exists (after initial it in strcpy)
    
    /* Initialize(memory save) the server_addr struct with the socket path */
    memset(&server_addr, 0, sizeof(server_addr));

    /* Set the server_addr struct values (Init its fields) */
    server_addr.sun_family = AF_UNIX;
    

    // Copy the socket path to the server_addr struct
    strncpy(server_addr.sun_path, socketPath.c_str(), sizeof(server_addr.sun_path) - 1);

    /* 
        Bind the server socket to the server_addr struct 
        bins system call is telling the OS that if sender process sends the data setined to socket file descriptor,
        then the OS should deliver the data to the server_addr struct (the server process).
    */
    if(bind(listeningSocket, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind");
        return -1;
    }

    cout<<"Bind Successfully "<<endl;

    /*
        Prepare for accepting connection. the backlog size is set to 20.
        so while one request is being processed other requests can be waiting.
    */
    if(listen(listeningSocket, 10) < 0)
    {
        perror("listen");
        return -1;
    }

    cout<<"Server is waiting for connections..."<<endl;
    
    // Define client 
    struct sockaddr_un client_address;
    // Define the client_address_len
    socklen_t client_address_len = sizeof(client_address);
    // Initialize the client_address struct to zero - memory clean
    memset(&client_address, 0, client_address_len);

    printf("Waiting for UDS connection...\n");
    
    // Accept a connection from the client
    int clientSocket = accept(listeningSocket, (struct sockaddr *)&client_address, &client_address_len);
    if (clientSocket < 0) {
        perror("Accept failed");
        close(listeningSocket);
        return -1;
    }
    
    cout<<"Connection accepted from client"<<endl;

    close(listeningSocket); // Close the listening socket as it's no longer needed
    return clientSocket; // Return the client socket
}

int startUDSServerDatagram(const std::string &socketPath)
{
    struct sockaddr_un server_addr; // Define the server_addr struct
    int fd_socket; 

    /* Create the server socket stream */
    fd_socket = socket(AF_UNIX, SOCK_DGRAM, 0);

    if(fd_socket < 0) // check if the socket was created successfully
    {
        perror("socket");
        return -1;
    }
    
    cout<<"Server Socket Created"<<endl;
    
    /* In case the program exited unexpectedly, the socket file may still exist. */
    unlink(socketPath.c_str()); // remove the socket file if it exists (after initial it in strcpy)
    
    /* Initialize(memory save) the server_addr struct with the socket path */
    memset(&server_addr, 0, sizeof(server_addr));

    /* Set the server_addr struct values (Init its fields) */
    server_addr.sun_family = AF_UNIX;
    

    // Copy the socket path to the server_addr struct
    strncpy(server_addr.sun_path, socketPath.c_str(), sizeof(server_addr.sun_path) - 1);
   
    // Bind the server socket to the server_addr struct
    if(bind(fd_socket, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind");
        return -1;
    }

    cout<<"Bind Successfully "<<endl;

    // Set the socket to non-blocking mode
    // if(!setSocketBlocking(fd_socket, false))
    // {
    //     perror("Failed to set socket to non-blocking mode");
    //     return -1;
    // }

    // Define client 
    struct sockaddr_un client_address; // Define the client_address struct
    socklen_t client_address_len = sizeof(client_address); // Define the client_address_len - size of type sockaddr_un
    memset(&client_address, 0, sizeof(client_address)); // Initialize the client_address struct to zero - memory clean
    char buffer[100]; // Define the buffer to store the message from the client

    cout<<"Server is waiting for connections..."<<endl;

    // Receive a message from the client
    ssize_t bytesReceived = recvfrom(fd_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_len);
    if(bytesReceived < 0)
    {
        perror("Failed to receive");
        close(fd_socket);
        return -1;
    }

    return fd_socket;
    }

