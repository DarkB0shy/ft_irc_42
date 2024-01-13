#ifndef SERVER_HPP
#define SERVER_HPP

#define MAXCLIENTS 10
#define WELCOMETOSERVER "WELCOME! Server listening on port "
#define CONNHANDLED "New connection from (ip, port): "
#define CLOSEDCONN "Connection closed from (ip, port): "
#define BUFFASIZE 1025

#define WRONGARGS "usage: ./ircserv #port connection_psswd"
#define WRONGPORT "the #port is invalid"
#define WRONGPASS "invalid password"
#define NOSOCKET "socket could not be created"
#define NOSOCKETFLAGS "socket could not be set to non-blocking mode"
#define PORTNOTBINDED "socket could not be binded to local port"
#define NOTLISTENING "socket could not start listening for incoming connections"
#define FDSETERROR "could not fetch file descriptors"
#define NEWCONNERR "could not establish new connection"
#define OUTERR "could not send message"
#define READERR "could not read message"


#define PSSWD_OK "password correct"
#define NNAME_OK "nickname set"
#define UNAME_OK "username set"
#define MSG_OK "message sent"
#define RPL_WELCOME "welcome to the Internet Relay Network"
#define RPL_YOURHOST "your host is ircserv"
#define RPL_CREATED "this server was created on new year's eve"
#define RPL_MYINFO "<ircserv> <version 0> <available user modes: join, nick, privmsg, topic (is allowed by the channel)> <available channel modes (only for chanOps): invite, kick, mode (i, t, k, o, l), topic>"
#define ERR_NOSUCHNICK "401"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_UNKOWNCOMMAND "421"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEOUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NOTREGISTRED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_ERRONEOUSUSER "usernames can have up to 9 characters, and cannot have ' ' or '@'"

#include "Client.hpp"
#include "Utils.hpp"
#include "Message.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>		// holds the struct sockaddr_in
#include <arpa/inet.h>      // inetitoa, ntohs, ...
#include <csignal>
#include <sys/select.h>     // FD_ZERO, ...

typedef struct	s_socket {
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
        void        sendMessage(int sfd, const std::string &message);
        void        sendGoodMessage(int sfd, std::string sReply, std::string nname);
        void        startServer(void);
        void        clearSocketsSet(void);
        void        resetSocketSet(void);
        void        runServer(void);
        int         getSocket(void) const;
		void        initClients(void);
        void        handleNewConnection(void);
        void        handleClientInput(Client &c);
        std::string handlePassCommand(Client &c, char * psswd);
        std::string handleNickCommand(Client &c, char * nname);
        std::string handleUserCommand(Client &c, char * user);
        std::string handlePrivMsgCommand(Client &c, char *privMsg);
        Server(void);
        Server(int portNumber, std::string pass);
        Server(const Server &s);
        Server &operator=(const Server &s);
        ~Server();
};

#endif