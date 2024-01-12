#include "../Includes/Client.hpp"

Client::Client (void) {_isReg = 0; _isChanOp = 0;}

Client::Client (const Client &c) {*this = c;}

Client &Client::operator=(const Client &c) {
    if (this == &c) return (*this);
    *this = Client(c);
    return (*this);
}

int Client::getSocketFd(void) const {return (_socketFd);}

std::string  Client::getIpAddress(void) const {return (_ipAddress);}

u_int16_t   Client::getPort(void) const {return (_port);}

int Client::getIsReg(void) const {return (_isReg);}

int Client::getIsChanOp(void) const {return (_isChanOp);}

std::string Client::getNickName(void) const {return (_nickName);}

void    Client::setSocketFd(int socketFd) {_socketFd = socketFd;}

void    Client::setIpAddress(std::string ipAddress) {_ipAddress = ipAddress;}

void    Client::setPort(u_int16_t port) {_port = port;}

void    Client::setIsReg(int reg) {_isReg = reg;}

void    Client::setIsChanOp(int chanOp) {_isChanOp = chanOp;}

void    Client::setNickName(std::string nname) {_nickName = nname;}

Client::~Client(void) {}
