#include "../Includes/Server.hpp"

Server::Server (void) {};

Server::Server (int portNumber, std::string pass) {
    _portNumber = portNumber;
    _pass = pass;
}

Server::Server (const Server &s) {*this = s;}

Server &Server::operator=(const Server &s) {
    if (this == &s) return (*this);
    *this = Server(s);
    return (*this);
}

int Server::getSocket() const { return (_socket); }

void    Server::initClients(void) {for (int i = 0; i < MAXCLIENTS; i++) _clients[i].setSocketFd(0);}

void    Server::startServer(void) {
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) std_errore(NOSOCKET);                 // Initializes socket fd (ipv4, tcp)
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1) std_errore(NOSOCKETFLAGS);                   // Sets non-blocking flag
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;                                                      // this allows connections from every network interface available
    _address.sin_port = htons(_portNumber);
    _addrlen = sizeof(_address);
    if (bind(_socket, (struct sockaddr *)&_address, _addrlen) < 0) std_errore(PORTNOTBINDED);
    if (listen(_socket, MAXCLIENTS) < 0) std_errore(NOTLISTENING);
    initClients();
    std::cout<<WELCOMETOSERVER<<_portNumber<<std::endl;
}

void    Server::resetSocketSet (void) {
    int clientFd;
    FD_ZERO(&_readFds);                                     // Clears socket set
    FD_SET(this->getSocket(), &_readFds);                   // Adds server socket to the set
    _maxFd = this->getSocket();
    for (int i = 0; i < MAXCLIENTS; i++) {
        clientFd = _clients[i].getSocketFd();
        if (clientFd > 0) FD_SET(clientFd, &_readFds);      // If its a valid fd it is added to the set
        if (clientFd > _maxFd) _maxFd = clientFd;           // The highest Fd is needed for select()
    }
}

void    Server::handleNewConnection(void) {
    t_socket    tempSocket;
	tempSocket.addrlen = sizeof(tempSocket.address);
	if ((tempSocket.socket = accept(this->getSocket(), (struct sockaddr *)&tempSocket.address, (socklen_t*)&tempSocket.addrlen)) < 0) {std_errore(NEWCONNERR);}
	for (int i = 0; i < MAXCLIENTS; i++) { if(_clients[i].getSocketFd() == 0) {                                                                                     // Adds the temp socket to this->_clients
			_clients[i].setSocketFd(tempSocket.socket);
			_clients[i].setIpAddress(inet_ntoa(tempSocket.address.sin_addr));
			_clients[i].setPort(ntohs(tempSocket.address.sin_port));
            _clients[i].setIsReg(0);
            _clients[i].setIsChanOp(0);
            std::cout<<CONNHANDLED<<_clients[i].getIpAddress()<<", "<<_clients[i].getPort()<<std::endl;
            break;
		}
	}
}

void    Server::runServer(void) {
    resetSocketSet();
	if (select(_maxFd + 1, &_readFds, NULL, NULL, NULL) == -1) std_errore(FDSETERROR);                                          // Waits for any I/O operation from the set of fds
    if (FD_ISSET(this->getSocket(), &_readFds)) handleNewConnection();                                                          // Checks if the main socket is inside the set, if it is it means there is a new connection
    for (int i = 0; i < MAXCLIENTS; i++) if (FD_ISSET(_clients[i].getSocketFd(), &_readFds)) handleClientInput(_clients[i]);    // If one of the client fds is inside the set it means there is some I/O operation coming through
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (_clients[i].getPsswdGuessed() && _clients[i].getNnameSet() && _clients[i].getUserSet()) {                           // RPL WELCOME !
            _clients[i].setIsReg(1);
            sendGoodMessage(_clients[i], RPL_WELCOME);
            sendGoodMessage(_clients[i], RPL_YOURHOST);
            sendGoodMessage(_clients[i], RPL_CREATED);
            std::string channelModes;
            if (!_clients[i].getIsChanOp()) {
                channelModes = "topic (if permitted by chanOp)>";
                std::string noOpReply = RPL_MYINFO + channelModes;
                sendGoodMessage(_clients[i], noOpReply);
            } else {
                channelModes = "invite, kick, mode (i, t, k, o, l), topic>";
                std::string opReply = RPL_MYINFO + channelModes;
                sendGoodMessage(_clients[i], opReply);
            }
        }
    }
}

void    Server::clearSocketsSet (void) {
	for (int fd = 0; fd <= _maxFd; fd++) if (FD_ISSET(fd, &_readFds)) close(fd);
	close(this->getSocket());
}

void	Server::sendMessage(const std::string &message, Client &c) {
    int send_res = send(c.getSocketFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
    if (send_res == -1) {switch (errno) {
            case EPIPE: return ;
            default: std_errore(OUTERR);
        }
    }
}

void    Server::sendGoodMessage(Client &c, std::string sReply) {                                // creates the server reply
    std::string r = "ircserv";
    if (!c.getNickName()[0]) r = r + " " + sReply + "\r\n";
    else r = r + " " + sReply + " " + c.getNickName() + "\r\n";
    sendMessage(r, c);
}

void    Server::handleClientInput(Client &c) {
    char    buffa[BUFFASIZE];
    int valread = recv(c.getSocketFd(), buffa, BUFFASIZE - 1, 0);
    if (valread == -1) std_errore(READERR);
    else if (valread == 0) {
        std::cout<<CLOSEDCONN<<c.getIpAddress()<<", "<<c.getPort()<<std::endl;
        c.setSocketFd(0);
        return ;
    }
    else {
        buffa[valread] = '\0';                                                                                // parsing ...
        int i = 0; while (buffa[i]) i++; if (i == 1 || i == 2) return ;                                       // empty line check or 1 character line check
        Message *newMssg;
        newMssg = newMssg->splittedMssg(buffa);
        if (!newMssg) return ;
        std::string serverReply;
        int flag = 0;                                                                                             // prefix check
        if (newMssg->getPrefix()[0]) {
            if (!c.getNickName()[0]) flag = 1;
            if (stringCompare(newMssg->getPrefix(), c.getNickName())) flag = 1;
        }
        if (!newMssg->getPrefix()[0] || !flag) {
            if (newMssg->getCommand()[0]) {
                if (!stringCompare(newMssg->getCommand(), "pass")) {                                                // password command
                    if (c.getIsReg()) serverReply = ERR_ALREADYREGISTERED;
                    else serverReply = handlePassCommand(c, newMssg->getParameters());
                }
                else if (!stringCompare(newMssg->getCommand(), "nick")) {
                    serverReply = handleNickCommand(c, newMssg->getParameters());                                   // nick command
                }
                else if (!stringCompare(newMssg->getCommand(), "user")) {                                           // user command
                    if (c.getUserSet()) serverReply = ERR_ALREADYREGISTERED;
                    else serverReply = handleUserCommand(c, newMssg->getParameters());
                }
                else ;
            } else return ;
            if (serverReply[0]) sendGoodMessage(c, serverReply);
        } else return ;
    }
}

std::string Server::handlePassCommand(Client &c, char * psswd) {
    if (!stringCompare(psswd, _pass)) {
        c.setPsswdGuessed(1) ;
        return (PSSWD_OK);
    } else return (ERR_PASSWDMISMATCH);
}

std::string Server::handleNickCommand(Client &c, char * nname) {
    if (!nname[0]) return (ERR_NONICKNAMEGIVEN);
    int i = 0;
    while (nname[i]) i++;
    if (i > 9) return (ERR_ERRONEOUSNICKNAME);
    std::string tempStr(nname);
    for (int j = 0; j < MAXCLIENTS; j++) if (!stringCompare(nname, _clients[j].getNickName())) return (ERR_NICKNAMEINUSE);
    c.setNickName(tempStr); c.setNnameSet(1); return (NNAME_OK);
}

std::string Server::handleUserCommand(Client &c, char * user) {
    if (!user[0]) return (ERR_NEEDMOREPARAMS);
    int flagOp = 0;
    int i = 0;
    if (user[i] == 'o' && user[1] && user[1] == ' ') {i = 2; flagOp = 1;}
    while (user[i]) {
        if (user[i] == ' ' || user[i] == '@' || user[i] == '\0' || user[i] == '\r' || user[i] == '\n') break;
        i++;
    }
    if (i != strlen(user) || i > 9) return (ERR_ERRONEOUSUSER);
    c.setUserName(user); c.setIsChanOp(flagOp); c.setUserSet(1); return (UNAME_OK);
}

Server::~Server (void) {}
