#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#define BUFFER_SIZE 1024
#include "udp_server.hpp"

using namespace std;


// Function to start a UDP server
int startUDPServer(int port)
{

    // get address info
    struct addrinfo addressInfo;

    // set up the hints structure
    memset(&addressInfo, 0, sizeof addressInfo);
    addressInfo.ai_socktype = SOCK_DGRAM;
    addressInfo.ai_family = AF_UNSPEC;
    addressInfo.ai_flags = AI_PASSIVE;

    // get address info for the server
    struct addrinfo *resolvedAddress;
    string port_str = to_string(port);
    int status;
    if ((status = getaddrinfo(NULL, port_str.c_str(), &addressInfo, &resolvedAddress)) != 0) 
    {
        cerr<<stderr<<"getaddrinfo: "<<gai_strerror(status)<<endl;;
        return -1;
    }

    int serverSocket = socket(resolvedAddress->ai_family, resolvedAddress->ai_socktype, resolvedAddress->ai_protocol);
    if (serverSocket < 0)
    {
        perror("Error creating socket");
        return -1;
    }

    int opt = 1; 
    
    // allow socket to be reused with the address and port
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(serverSocket);
        return -1;
    }

    cout<<"Waiting for bind..."<<endl;

    // Bind the socket to the specified address and port
    if (bind(serverSocket, resolvedAddress->ai_addr, resolvedAddress->ai_addrlen) < 0)
    {
        perror("Bind Failed");
        close(serverSocket);
        return -1;
    }

    cout<<"Binded Successfully"<<endl;
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
    }

    freeaddrinfo(resolvedAddress);  // free the linked list

    return serverSocket;
}