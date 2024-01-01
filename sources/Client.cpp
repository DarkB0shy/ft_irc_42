#include "../includes/Client.hpp"

Client::Client (void) {}

Client::Client(int socketFd) {_socketFd = socketFd;}

Client::Client (const Client &c) {*this = c;}

Client &Client::operator=(const Client &c) {
    if (this == &c)
        return (*this);
    *this = Client(c);
    return (*this);
}

int Client::getSocketFd(void) const {return (_socketFd);}

void    Client::setSocketFd(int socketFd) {_socketFd = socketFd;}

void    Client::setIpAddress(char* ipAddress) {_ipAddress = ipAddress;}

void    Client::setPort(u_int16_t port) {_port = port;}

Client::~Client(void) {}
