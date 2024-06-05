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


using namespace std;

// Function to start a UDP client
int startUDPClient(const string &hostname, int port) {

    // get address info
    struct addrinfo addressInfo;

    // set up the hints structure
    memset(&addressInfo, 0, sizeof addressInfo);
    addressInfo.ai_socktype = SOCK_DGRAM;
    addressInfo.ai_family = AF_UNSPEC;

    // get address info for the server
    struct addrinfo *resolvedAddress;
    string port_str = to_string(port);
    int status;
    if ((status = getaddrinfo(hostname.c_str(), port_str.c_str(), &addressInfo, &resolvedAddress)) != 0) 
    {
        cerr<<stderr<<"getaddrinfo: "<<gai_strerror(status)<<endl;;
        return -1;
    }

    // loop through all the results and make a socket (first we encounter that works)
    struct addrinfo *addrLooper;
    int sockfd;
    for (addrLooper = resolvedAddress; addrLooper != NULL; addrLooper = addrLooper->ai_next) 
    {
        if ((sockfd = socket(addrLooper->ai_family, addrLooper->ai_socktype, addrLooper->ai_protocol)) < 0) 
        {
            perror("error creating socket");
            continue;
        }
        // "connect" to the server - so if we use sendto/recvfrom, we don't need to specify the server address
        if(connect(sockfd, addrLooper->ai_addr, addrLooper->ai_addrlen) == 0)
        {
            break;  // if we get here, we must have connected successfully
        }
        else
        {
            cerr<<stderr<<"failed to connect"<<endl;
        }
    }

    if (addrLooper == NULL) {
        cerr<<stderr<<"failed to connect"<<endl;
        return -1;
    }


    freeaddrinfo(resolvedAddress);  // free the linked list
    return sockfd;
}
