#include "../Includes/Client.hpp"

Client::Client (void) {_psswdGuessed = 0; _nnameSet = 0; _userSet = 0; _hasBeenWelcomed = 0;}

Client::Client (const Client &c) {*this = c;}

Client &Client::operator=(const Client &c) {
    if (this == &c) return (*this);
    *this = Client(c);
    return (*this);
}

int Client::getSocketFd(void) const {return (_socketFd);}

std::string  Client::getIpAddress(void) const {return (_ipAddress);}

u_int16_t   Client::getPort(void) const {return (_port);}

std::string Client::getNickName(void) const {return (_nickName);}

std::string Client::getUserName(void) const {return (_userName);}

int Client::getPsswdGuessed(void) const {return (_psswdGuessed);}

int Client::getNnameSet(void) const {return (_nnameSet);}

int Client::getUserSet(void) const {return (_userSet);}

int Client::getHasBeenWelcomed(void) const {return _hasBeenWelcomed;}

void    Client::setSocketFd(int socketFd) {_socketFd = socketFd;}

void    Client::setIpAddress(std::string ipAddress) {_ipAddress = ipAddress;}

void    Client::setPort(u_int16_t port) {_port = port;}

void    Client::setNickName(std::string nname) {_nickName = nname;}

void    Client::setUserName(std::string uname) {_userName = uname;}

void    Client::setPsswdGuessed(int pGuessed) {_psswdGuessed = pGuessed;}

void    Client::setNnameSet(int nnSet) {_nnameSet = nnSet;}

void    Client::setUserSet(int uSet) {_userSet = uSet;}

void    Client::setHasBeenWelcomed(int hbw) {_hasBeenWelcomed = hbw;}

Client::~Client(void) {}
