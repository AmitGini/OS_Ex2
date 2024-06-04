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
#include "udp_server.hpp"
#define BUFFER_SIZE 1024

using namespace std;


// Function to start a UDP server
int startUDPServer(int port)
{
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        perror("Error creating socket");
        return -1;
    }else cout<<"Server UDP Socket Created On Port: "<< port << endl;
    
    int opt = 1; 
    // allow socket to be reused
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(serverSocket);
        return -1;
    }

    struct sockaddr_in server_addr; // Struct sockaddr_in is defined in the <netinet/in.h> header file.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // The server will listen on all available network interfaces.
    server_addr.sin_port = htons(port); // htons() function ensures that the port number is properly formatted for network communication, regardless of the byte order of the host machine.
   
    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bined Failed");
        close(serverSocket);
        return -1;
    }

    // Define Client
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    cout<<"Ready to receive.."<<endl;
    cout<<"Enter ok"<<endl;

        ssize_t bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_len);
        if (bytesReceived < 0) {
            perror("Failed to receive");
            close(serverSocket);
            return -1;   
        }
    
    cout<<"Connected to client"<<endl;

    return serverSocket;
}