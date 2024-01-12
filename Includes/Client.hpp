#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>

class Client {
    private:
        int                 _socketFd;
        std::string         _ipAddress;
        u_int16_t           _port;
        int                 _isReg;                             // set to 0 inside handleNewConnection
        int                 _isChanOp;                          // set to 0 inside handleNewConnection
        std::string         _nickName;
    public:
        int             getSocketFd(void) const;
        std::string     getIpAddress(void) const;
        u_int16_t       getPort(void) const;
        int             getIsReg(void) const;
        int             getIsChanOp(void) const;
        std::string     getNickName(void) const;
        void            setSocketFd(int socketFd);
        void            setIpAddress(std::string ipAddress);
        void            setPort(u_int16_t port);
        void            setIsReg(int reg);
        void            setIsChanOp(int chanOp);
        void            setNickName(std::string nname);
        Client(void);
        Client(const Client &c);
        Client &operator=(const Client &c);
        ~Client();
};

#endif