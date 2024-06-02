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

#include "tcp_client.hpp"

using namespace std;

int startTCPClient(const string &hostname, int port) {
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient < 0) {
        perror("Socket creation failed");
        return -1;
    }
    
    // Create server address struct
    struct sockaddr_in serverAddress;  // Struct sockaddr_in is defined in the <netinet/in.h> header file.
    struct hostent *server;
    memset(&serverAddress, 0, sizeof(serverAddress));

     // Resolve hostname to IP address
    server = gethostbyname(hostname.c_str());
    if (server == nullptr) {
        cerr << "ERROR, no such host: " << hostname << endl;
        close(socketClient);
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    // bcopy copies the IP address from hostent to the sockaddr_in structure.
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port); // htons() function ensures that the port number is properly formatted for network communication, regardless of the byte order of the host machine.

    cout << "Connecting " << hostname << " on Port: " << port << endl;
    
    if (connect(socketClient, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("ERROR connecting");
        close(socketClient);
        return -1;
    }
    
    cout << "Connected " << hostname << " on Port: " << port << endl;
    
    return socketClient;
}

