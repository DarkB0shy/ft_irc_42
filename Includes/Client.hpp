#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>

class Client {
    private:
        int                 _hasBeenWelcomed;                   // set to 0 inside constructor, set to 1 once the client has registred (has a nick and a user set, and has guessed the connection password)
        std::string         _ipAddress;
        std::string         _nickName;
        int                 _nnameSet;                          // used for handling new registrations, set to 0 inside constructor, set to 1 once the nickname has been set
        u_int16_t           _port;
        int                 _psswdGuessed;                      // used for handling new registrations, set to 0 inside constructor
        int                 _socketFd;
        std::string         _userName;
        int                 _userSet;                           // used for handling new registrations, set to 0 inside constructor
    public:
        Client(void);
        Client(const Client &c);
        Client &operator=(const Client &c);
        int             getHasBeenWelcomed(void) const;         // get methods
        std::string     getIpAddress(void) const;
        std::string     getNickName(void) const;
        int             getNnameSet(void) const;
        u_int16_t       getPort(void) const;
        int             getPsswdGuessed(void) const;
        int             getSocketFd(void) const;
        std::string     getUserName(void) const;
        int             getUserSet(void) const;
        void            setHasBeenWelcomed(int hbw);            // set methods
        void            setIpAddress(std::string ipAddress);
        void            setNickName(std::string nname);
        void            setNnameSet(int nnSet);
        void            setSocketFd(int socketFd);
        void            setPort(u_int16_t port);
        void            setPsswdGuessed(int pGuessed);
        void            setUserName(std::string uname);
        void            setUserSet(int uSet);
        ~Client();
};

#endif