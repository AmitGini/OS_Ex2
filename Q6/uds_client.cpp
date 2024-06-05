
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include "uds_client.hpp"

using namespace std;

int startUDSClientStream(const std::string &socketPath)
{
    
    int socketClient= socket(AF_UNIX, SOCK_STREAM, 0); //Create the client socket stream
    if(socketClient < 0) // check if the socket was created successfully
    {
        perror("Socket creation Failed");
        return -1;
    }

    cout<<"Client Socket Created"<<endl;

    const char* socket_path = socketPath.c_str();// Convert the string to a const char*
    struct sockaddr_un socket_addr; // Define the server_addr struct
    socket_addr.sun_family = AF_UNIX; // Set the server_addr struct values (Init its fields)
    strncpy(socket_addr.sun_path, socket_path, sizeof(socket_addr.sun_path) - 1); // Copy the socket path to the server_addr struct

    /* 
        Connect the client socket to the server_addr struct 
        connect system call is telling the OS that if the client process sends the data setined to socket file descriptor,
        then the OS should deliver the data to the server_addr struct (the server process).
    */
    if(connect(socketClient, (const struct sockaddr *) &socket_addr, sizeof(socket_addr)) == -1)
    {
        perror("Failed to connect");
        close(socketClient);
        return -1;
    }

    cout<<"Connected Successfully "<<endl;
    return socketClient; // Get Input from the client
}

// Create a Unix Domain Socket client using datagram
int startUDSClientDatagram(const string &socketPath)
{
    int recvfromSocket = socket(AF_UNIX, SOCK_DGRAM, 0); // Create the client socket datagram
    
    if(recvfromSocket < 0) // check if the socket was created successfully
    {
        perror("Socket creation Failed");
        return -1;
    }
    
    cout<<"Client Socket Created"<<endl;

    const char* socket_path = socketPath.c_str(); // Convert the string to a const char*
    struct sockaddr_un socket_addr; // Define the server_addr struct
    socket_addr.sun_family = AF_UNIX; // Set the server_addr struct values 
    strncpy(socket_addr.sun_path, socket_path, sizeof(socket_addr.sun_path) - 1); // Copy the socket path to the server_addr struct
    
    
    if(connect(recvfromSocket, (const struct sockaddr *) &socket_addr, sizeof(socket_addr)) == -1)
    {
        perror("Failed to connect");
        close(recvfromSocket);
        return -1;
    }
    
    return recvfromSocket;
}
