
#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <string>
using std::string;

// create tcp server
int startTCPServer(int port); 

// Accept a connection from the client
int acceptTCPConnection(int listeningSocket);

#endif