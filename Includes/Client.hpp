#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>

class Client {
    private:
        int                 _socketFd;
        std::string         _ipAddress;
        u_int16_t           _port;
    public:
        int             getSocketFd(void) const;
        std::string     getIpAddress(void) const;
        u_int16_t       getPort(void) const;
        void            setSocketFd(int socketFd);
        void            setIpAddress(std::string ipAddress);
        void            setPort(u_int16_t port);
        Client(void);
        Client(int socketFd);
        Client(const Client &c);
        Client &operator=(const Client &c);
        ~Client();
};

#endif