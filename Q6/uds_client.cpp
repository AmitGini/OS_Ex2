
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
    
    /* Create the client socket stream */
    int socketClient= socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketClient < 0) // check if the socket was created successfully
    {
        perror("Socket creation Failed");
        return -1;
    }

    // Convert the string to a const char* to be used in the connect system call
    const char* socket_path = socketPath.c_str();
    
    cout<<"Client Socket Created"<<endl;
    
    // Define the server_addr struct
    struct sockaddr_un serverAddress;

    /* Initialize(memory save) the server_addr struct with the socket path */
    memset(&serverAddress, 0, sizeof(serverAddress));


    /* Set the server_addr struct values (Init its fields) */
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, socket_path, sizeof(serverAddress.sun_path) - 1);

    /* 
        Connect the client socket to the server_addr struct 
        connect system call is telling the OS that if the client process sends the data setined to socket file descriptor,
        then the OS should deliver the data to the server_addr struct (the server process).
    */
   
    if(connect(socketClient, (const struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Failed to connect");
        close(socketClient);
        return -1;
    }

    cout<<"Connected Successfully "<<endl;
    
    /* Get Input from the client*/;
    return socketClient;
}