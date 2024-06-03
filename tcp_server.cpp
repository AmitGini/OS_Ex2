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

#include "tcp_server.hpp"

using namespace std;

int startTCPServer(int port) {
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);  // Create a TCP socket for IPv4
    
    if (listeningSocket < 0)
    {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1; // Indicates that we want to enable the SO_REUSEADDR and SO_REUSEPORT options.
    // Setting SO_REUSEADDR allows the program to reuse the socket with the same address and port number
    // Several processes can bind to the same port and listen to incoming connections, 
    // which enables load balancing between the processes automatically.
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(listeningSocket);
        return -1;
    }
    
    cout << "Starting TCP server on port " << port << endl;
    struct sockaddr_in address;  // Struct sockaddr_in is defined in the <netinet/in.h> header file.

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // The server will listen on all available network interfaces.
    address.sin_port = htons(port); // htons() function ensures that the port number is properly formatted for network communication, regardless of the byte order of the host machine.

    // Bind the socket to the specified address and port
    if (bind(listeningSocket, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind() failed");
        close(listeningSocket);
        return -1;
    }

    // Listen for incoming connections - 1 TCP connections
    if (listen(listeningSocket, 1) < 0) 
    {
        perror("listen() failed");
        close(listeningSocket);
        return -1;
    } // listen to 

    return listeningSocket;
}

int acceptTCPConnection(int listeningSocket) 
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Accept a connection from the client
    cout<<"Waiting for TCP connection..."<<endl;
    int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    
    if (clientSocket < 0) 
    {
        perror("accept() failed");
        close(listeningSocket);
        return -1;
    }

    return clientSocket;
}



