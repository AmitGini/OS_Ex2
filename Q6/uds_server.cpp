#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "uds_server.hpp"

using namespace std;

// Create a Unix Domain Socket server using stream
int startUDSServerStream(const string &socketPath)
{
    
    struct sockaddr_un server_addr; 
    int listeningSocket;
    int data_socket;


    /* Create the server socket stream */
    listeningSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(listeningSocket < 0) // check if the socket was created successfully
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    cout<<"Server Socket Created"<<endl;
    
    /* Initialize(memory save) the server_name struct with the socket path */
    memset(&server_addr, 0, sizeof(server_addr));

    /* Set the server_name struct values (Init its fields) */
    server_addr.sun_family = AF_UNIX;
    
    /* In case the program exited unexpectedly, the socket file may still exist. */
    unlink(socketPath.c_str()); // remove the socket file if it exists (after initial it in strcpy)
    
    // Copy the socket path to the server_addr struct
    strncpy(server_addr.sun_path, socketPath.c_str(), sizeof(server_addr.sun_path) - 1);

    /* 
        Bind the server socket to the server_name struct 
        bins system call is telling the OS that if sender process sends the data setined to socket file descriptor,
        then the OS should deliver the data to the server_name struct (the server process).
    */
    if(bind(listeningSocket, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    cout<<"Bind Successfully "<<endl;

    /*
        Prepare for accepting connection. the backlog size is set to 20.
        so while one request is being processed other requests can be waiting.
    */
    if(listen(listeningSocket, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout<<"Server is waiting for connections..."<<endl;
    
    // Define client 
    struct sockaddr_un client_address;
    socklen_t client_address_len;
    memset(&client_address, 0, sizeof(client_address));

    // Accept a connection from the client
    printf("Waiting for UDS connection...\n");
    int clientSocket = accept(listeningSocket, (struct sockaddr *)&client_address, &client_address_len);
    if (clientSocket < 0) {
        perror("Accept failed");
        close(listeningSocket);
        return -1;
    }
    
    cout<<"Connection accepted from client"<<endl;

    close(listeningSocket);
    return clientSocket;
}
