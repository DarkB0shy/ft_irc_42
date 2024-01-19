#ifndef SERVER_HPP
#define SERVER_HPP

#define MAXCLIENTS 10
#define MAXCHANS 2
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
#define RPL_WELCOME "welcome to the Internet Relay Network"
#define RPL_YOURHOST "your host is ircserv"
#define RPL_CREATED "this server was created on new year's eve"
#define RPL_MYINFO "<ircserv> <version 0> <available user modes: invite, join, nick, privmsg, topic (if allowed by chan ops)> <available channel modes (only for chan ops): kick, mode (i, t, k, o, l), topic>"
#define RPL_NO_TOPIC "no topic set yet"
#define RPL_TOPIC "the topic of the channel is "
#define RPL_NAMEREPLY "currently online members of the channel: "
#define MSG_OK "message sent"
#define CHAN_CREATED "channel created"
#define CHAN_JOINED "you are now a member of the channel"
#define JOINZERO "you left every channel you ever joined"
#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_UNKOWNCOMMAND "421"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEOUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"
#define ERR_NOTREGISTRED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_KEYSET "467"
#define ERR_CHANNELLISFULL "471"
#define ERR_UNKOWNMODE "472"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_ERRONEOUSUSER "usernames can have up to 9 characters, and cannot have ' ' or '@'"
#define ERR_ERRONEOUSCHANNAME "channel names can have up to 50 characters, must start with &, and cannot have ' ' or ','"
#define ERR_ALREADYONCHAN "you have already joined the channel"
#define ERR_TOOMANYPARAMETERS "mode takes a maximum of 3 parameters, -l, +i, -i, +t, -t take no arguments instead"
#define ERR_INVALIDCHANAME "you are not a member of that channel"

#include "Client.hpp"
#include "Utils.hpp"
#include "Message.hpp"
#include "Channel.hpp"
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
        Channel                         _channels[MAXCHANS];
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
        std::string handlePassCommand(Client &c, char * pass);
        std::string handleNickCommand(Client &c, char * nick);
        std::string handleUserCommand(Client &c, char * user);
        std::string handlePrivMsgCommand(Client &c, char *privMsg);
        std::string handleJoinCommand(Client &c, char * join);
        void        sendJoinNotice(int a, Client &c, std::string tempChanName);
        void        sendOpNotice(std::string tempChoppa, std::string serverReply, Client &c);
        int         chanExists(std::string chanName);
        int         getNewChanIndex(void);
        void        createChan(std::string chanName, std::string chanFounder, int a);
        std::string handleModeCommandOne(Client &c, char * mode);
        std::string handleModeCommandTwo(Client &c, char * mode);
        std::string handleTopicCommand(Client &c, char * topic);
        std::string handleInviteCommand(Client &c, char * invite);
        std::string handleKickCommand(Client &c, char * nick);
        Server(void);
        Server(int portNumber, std::string pass);
        Server(const Server &s);
        Server &operator=(const Server &s);
        ~Server();
};

#endif