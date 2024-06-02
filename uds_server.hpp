#ifndef UDS_SERVER_HPP
#define UDS_SERVER_HPP

#include <string>
#include <vector>
using namespace std;

// create unix domain socket server using stream
int startUDSServerStream(const std::string &socketPath);

// create unix domain socket server using datagram 
int startUDSServerDatagram(const std::string &socketPath);

bool setSocketBlocking(int fd, bool blocking);

#endif