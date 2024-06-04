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
#include "udp_client.hpp"
#define BUFFER_SIZE 1024


using namespace std;

// Function to start a UDP client
int startUDPClient(const string &hostname, int port) {
        // get address info
    struct addrinfo hints, *res, *p;
    int status;
    int sockfd;

    // set up the hints structure
    memset(&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    // get address info
    string port_str = to_string(port);
    if ((status = getaddrinfo(hostname.c_str(), port_str.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    // loop through the results and connect to the first we can
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("error creating socket");
            continue;
        }
        // "connect" to the server - so if we use sendto/recvfrom, we don't need to specify the server address
        connect(sockfd, p->ai_addr, p->ai_addrlen);

        break;  // if we get here, we must have connected successfully
    }

    if (p == NULL) {
        cerr<<stderr<<"failed to connect"<<endl;
        return -1;
    }

    freeaddrinfo(res);  // free the linked list

    return sockfd;
}
    /*
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        return -1;
    }
    
    //Create server address struct
    struct sockaddr_in server_address; // Struct sockaddr_in is defined in the <netinet/in.h> header file.
    struct hostent *server = gethostbyname(hostname.c_str());
    if (server == nullptr) {
        cerr << "Error: No such host" << endl;
        close(clientSocket);
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htonl(port);
    memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length);

    char buffer[BUFFER_SIZE];
    ssize_t r;
    while ((r = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[r] = '\0';
            ssize_t s = sendto(clientSocket, buffer, r, 0, (struct sockaddr *)&server_address, sizeof(server_address));
            if (s<0){
                perror("Error sending");
                close(clientSocket);
                return -1;
            }
    }

    return clientSocket;
}
*/