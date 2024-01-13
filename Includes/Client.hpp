#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>

class Client {
    private:
        int                 _socketFd;
        std::string         _ipAddress;
        u_int16_t           _port;
        std::string         _nickName;
        std::string         _userName;
        int                 _psswdGuessed;                      // used for handling new registrations, set to 0 inside constructor
        int                 _nnameSet;                          // used for handling new registrations, set to 0 inside constructor
        int                 _userSet;                           // used for handling new registrations, set to 0 inside constructor
        int                 _hasBeenWelcomed;
    public:
        int             getSocketFd(void) const;
        std::string     getIpAddress(void) const;
        u_int16_t       getPort(void) const;
        std::string     getNickName(void) const;
        std::string     getUserName(void) const;
        int             getPsswdGuessed(void) const;
        int             getNnameSet(void) const;
        int             getUserSet(void) const;
        int             getHasBeenWelcomed(void) const;
        void            setSocketFd(int socketFd);
        void            setIpAddress(std::string ipAddress);
        void            setPort(u_int16_t port);
        void            setNickName(std::string nname);
        void            setUserName(std::string uname);
        void            setPsswdGuessed(int pGuessed);
        void            setNnameSet(int nnSet);
        void            setUserSet(int uSet);
        void            setHasBeenWelcomed(int hbw);
        Client(void);
        Client(const Client &c);
        Client &operator=(const Client &c);
        ~Client();
};

#endif