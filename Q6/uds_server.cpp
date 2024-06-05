#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "uds_server.hpp"

#define BUFFER_SIZE 1024

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
        Prepare for accepting connection. the backlog size is set to 1.
        so while one request is being processed other requests can be waiting.
    */
    if(listen(listeningSocket, 1) < 0)
    {
        perror("listen");
        return -1;
    }

    cout<<"UDS Server is waiting for connections..."<<endl;
    
    return listeningSocket; // Return the client socket
}

int startUDSServerDatagram(const std::string &socketPath)
{
    int serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0); // Create the server socket stream 

    if(serverSocket < 0) // check if the socket was created successfully
    {
        perror("socket");
        return -1;
    }
    else cout<<"Server UDS Socket Created"<<endl;
    
    unlink(socketPath.c_str()); // remove the socket file if it exists (after initial it in strcpy)
    struct sockaddr_un server_addr; // Define the server_addr struct
    server_addr.sun_family = AF_UNIX; // Set the server_addr struct values (Init its fields)
    strncpy(server_addr.sun_path, socketPath.c_str(), sizeof(server_addr.sun_path) - 1); // Copy the socket path to the server_addr struct
   
    // Bind the server socket to the server_addr struct
    if(bind(serverSocket, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind");
        close(serverSocket);
        return -1;
    } 
    else cout<<"Bind Successfully "<<endl;

    struct sockaddr_un client_address; // Define the client_address struct
    socklen_t client_address_len = sizeof(client_address); // Define the client_address_len - size of type sockaddr_un
    char buffer[BUFFER_SIZE]; // Define the buffer to store the message from the client
    cout<<"Server is waiting for connections..."<<endl;

    //Receive a message from the client
    ssize_t bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_len);
    
    if(bytesReceived < 0)
    {
        perror("Failed to receive");
        close(serverSocket);
        return -1;
    }else cout<<"Data Received Successfully - Num of Bytes: "<< bytesReceived <<endl;

    return serverSocket;
    }

