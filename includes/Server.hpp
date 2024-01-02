#ifndef SERVER_HPP
#define SERVER_HPP

#define MAXCONNECTIONS 50
#define MAXCLIENTS 10
#define WELCOMETOSERVER "WELCOME! Server listening on port "
#define CONNHANDLED "New connection from (ip, port): "
#define CLOSEDCONN "Connection closed from (ip, port): "
#define BUFFASIZE 1025

#define ERROR "Error: "
#define OUTERR "could not send message"
#define READERR "could not read message"
#define WRONGARGS "usage: ./ircserv #port connection_pssw"
#define NOSOCKET "socket could not be created"
#define NOSOCKETFLAGS "socket could not be set to non-blocking mode"
#define PORTNOTBINDED "socket could not be binded to local port"
#define NOTLISTENING "socket could not start listening for incoming connections"
#define FDSETERROR "could not fetch proper file descriptors"
#define NEWCONNERR "could not establish new connection"

#include "Client.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>		// helds the struct sockaddr_in
#include <arpa/inet.h>      // inetitoa, ntohs, ...
#include <csignal>
#include <sys/select.h>     // FD_ZERO, ...
#include <cstring>

typedef struct	s_socket
{
	int					port;
	int					socket;
	int					fd;
	int					addrlen;
	struct sockaddr_in	address;
}				t_socket;

class   Server {
    private:
        int                             _portNumber;
        int                             _addrlen;
        int                             _socket;
        int                             _maxFd;
        struct  sockaddr_in             _address;
        fd_set                          _readFds;
        std::string                     _hostname;
        std::string                     _pass;
        Client                          _clients[MAXCLIENTS];
    public:
        void    sendMessage(const int socket, const std::string &message);
        void    startServer(void);
        void    clearSocketsSet(void);
        void    resetSocketSet(void);
        void    runServer(void);
        int     getSocket(void) const;
		void    initClients(void);
        void    handleNewConnection(void);
        void    handleClientInput(Client &c);
        Server(void);
        Server(int portNumber, std::string pass);
        Server(const Server &s);
        Server &operator=(const Server &s);
        ~Server();
};

void    std_errore(const char *err);
void	sendMessage(const int socket, const std::string &message);

#endif