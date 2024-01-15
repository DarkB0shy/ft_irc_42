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
	for (int i = 0; i < MAXCLIENTS; i++) { if (_clients[i].getSocketFd() == 0) {                                                                                     // Adds the temp socket to this->_clients
			_clients[i].setSocketFd(tempSocket.socket);
			_clients[i].setIpAddress(inet_ntoa(tempSocket.address.sin_addr));
			_clients[i].setPort(ntohs(tempSocket.address.sin_port));
            _clients[i].setHasBeenWelcomed(1);
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
    for (int i = 0; i < MAXCLIENTS; i++) {                                                                                      // RPL_WELCOME
        if (!_clients[i].getHasBeenWelcomed() && _clients[i].getPsswdGuessed() && _clients[i].getNnameSet() && _clients[i].getUserSet()) {
            _clients[i].setHasBeenWelcomed(1);
            sendGoodMessage(_clients[i].getSocketFd(), RPL_WELCOME, _clients[i].getNickName());
            sendGoodMessage(_clients[i].getSocketFd(), RPL_YOURHOST, _clients[i].getNickName());
            sendGoodMessage(_clients[i].getSocketFd(), RPL_CREATED, _clients[i].getNickName());
            sendGoodMessage(_clients[i].getSocketFd(), RPL_MYINFO, _clients[i].getNickName());
            }
            break ;
    }
}

void    Server::clearSocketsSet (void) {
	for (int fd = 0; fd <= _maxFd; fd++) if (FD_ISSET(fd, &_readFds)) close(fd);
	close(this->getSocket());
}

void	Server::sendMessage(int sfd, const std::string &message) {                                              // sends the message. MSG_NOSIGNAL is needed otherwise one client disconnection causes the disconnection of the server as well. This is why if EPIPE error is returned the server does nothing
    int send_res = send(sfd, message.c_str(), message.size(), MSG_NOSIGNAL);
    if (send_res == -1) {switch (errno) {
            case EPIPE: return ;
            default: std_errore(OUTERR);
        }
    }
}

void    Server::sendGoodMessage(int sfd, std::string sReply, std::string nname) {                                // creates the server reply
    std::string r = "ircserv";
    if (!nname[0]) r = r + " " + sReply + "\r\n";
    else r = r + " " + sReply + " " + nname + "\r\n";
    sendMessage(sfd, r);
}

void    Server::handleClientInput(Client &c) {
    char    buffa[BUFFASIZE];
    int valread = recv(c.getSocketFd(), buffa, BUFFASIZE - 1, 0);
    if (valread == -1) std_errore(READERR);
    else if (valread == 0) {                                                                                    // valread returns 0 when the connection to the socket is lost
        std::cout<<CLOSEDCONN<<c.getIpAddress()<<", "<<c.getPort()<<std::endl;
        c.setSocketFd(0);
        return ;
    }
    else {
        buffa[valread] = '\0';                                                                                // parsing ...
        int i = 0; while (buffa[i]) i++; if (i == 1 || i == 2) return ;                                       // empty line check or 1 character line check
        Message *newMssg;
        newMssg = newMssg->splittedMssg(buffa);                                                                   // splits the message into (prefix) command and parameters
        if (!newMssg) return ;
        std::string serverReply;
        int flag = 0;                                                                                             // prefix check
        if (newMssg->getPrefix()[0]) {
            if (!c.getNickName()[0]) flag = 1;
            if (stringCompare(newMssg->getPrefix(), c.getNickName())) flag = 1;
        }    
        if (!newMssg->getPrefix()[0] || !flag) {
            if (newMssg->getCommand()[0]) {
                if (!stringCompare(newMssg->getCommand(), "pass")) { 
                    serverReply = handlePassCommand(c, newMssg->getParameters());                                   // pass command
                }
                else if (!stringCompare(newMssg->getCommand(), "nick")) {
                    serverReply = handleNickCommand(c, newMssg->getParameters());                                   // nick command
                }
                else if (!stringCompare(newMssg->getCommand(), "user")) {                                           // user command
                    serverReply = handleUserCommand(c, newMssg->getParameters());
                }
                else if (!stringCompare(newMssg->getCommand(), "privmsg")) {                                        // privmsg command
                    serverReply = handlePrivMsgCommand(c, newMssg->getParameters());
                }
                else if (!stringCompare(newMssg->getCommand(), "join")) {
                    serverReply = handleJoinCommand(c, newMssg->getParameters());
                }
                else if (c.getHasBeenWelcomed()) serverReply = ERR_UNKOWNCOMMAND;
                else ;
            } else ;
            if (serverReply[0]) sendGoodMessage(c.getSocketFd(), serverReply, c.getNickName());
        } else ;
    }
}

std::string Server::handlePassCommand(Client &c, char * pass) {
    if (c.getHasBeenWelcomed()) return (ERR_ALREADYREGISTERED);
    if (!stringCompare(pass, _pass)) {
        c.setPsswdGuessed(1) ;
        return (PSSWD_OK);
    } else return (ERR_PASSWDMISMATCH);
}

std::string Server::handleNickCommand(Client &c, char * nick) {
    if (!nick[0]) return (ERR_NONICKNAMEGIVEN);
    int i = 0;
    while (nick[i]) i++;
    if (i > 9) return (ERR_ERRONEOUSNICKNAME);                      // nick names can be made up of any character but have a maximum size of 9
    std::string tempStr(nick);
    for (int j = 0; j < MAXCLIENTS; j++) if (!stringCompare(nick, _clients[j].getNickName())) return (ERR_NICKNAMEINUSE);          // it is very common for several people to want the same nickname
    c.setNickName(tempStr); c.setNnameSet(1); return (NNAME_OK);
}

std::string Server::handleUserCommand(Client &c, char * user) {
    if (c.getUserSet() || c.getHasBeenWelcomed()) return (ERR_ALREADYREGISTERED);                       // user names can not have '@' or ' ' and have a maximum size of 9
    if (!user[0]) return (ERR_NEEDMOREPARAMS);                    
    int j = 0;
    while (user[j] && user[j] == ' ') j++;
    if (j >= 1 || strlen(user) > 9) return (ERR_ERRONEOUSUSER);
    int i = 0;
    while (user[i]) {
        if (user[i] == ' ' || user[i] == '@') {
            return (ERR_ERRONEOUSUSER);
        }
        i++;
    }
    c.setUserName(user); c.setUserSet(1); return (UNAME_OK);
}

std::string Server::handlePrivMsgCommand(Client &c, char * privMsg) {
    if (!c.getHasBeenWelcomed()) return (ERR_NOTREGISTRED);
    if (!privMsg[0]) return (ERR_NORECIPIENT);
    int i = 0;
    while (privMsg[i] && privMsg[i] != ' ') i++;                                    // privmsg syntax check: does not matter if dest is valid, just checks if there is text to send
    if (privMsg[i] && privMsg[i] == ' ') i++;
    if (!privMsg[i] || privMsg[i] == ' ') return (ERR_NOTEXTTOSEND);
    std::string tempPrivMsg(privMsg);
    std::string msgDest;
    int         destSocket;
    int         flag = 0;
    std::string toSend = c.getNickName() + " privmsg " +  + ": " + tempPrivMsg.substr(tempPrivMsg.find(' '), tempPrivMsg.length()) + "\r\n\0";
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (!stringCompareTheReturn(tempPrivMsg.substr(0, tempPrivMsg.find(' ')), _clients[i].getNickName())) {         // this is pretty self explanatory a tegridy check on dest needed to be executed
            msgDest = _clients[i].getNickName();
            destSocket = _clients[i].getSocketFd();
            sendMessage(destSocket, toSend);
            break;
        }
        if (i == MAXCLIENTS - 1) {
            for (int a = 0; a < MAXCHANS; a++) {
                if (!stringCompareTheReturn(tempPrivMsg.substr(0, tempPrivMsg.find(' ')), _channels[a].getChanName())) {
                    flag = 1;
                    std::string chanMsg = c.getNickName() + _channels[a].getChanName() + " privmsg " +  + ": " + tempPrivMsg.substr(tempPrivMsg.find(' '), tempPrivMsg.length()) + "\r\n\0";
                    for (int b = 0; b < MAX_CHANMEMBERS; b++) { 
                        int veryTempSocket = 0;
                        if (_channels[a].isChanMember(_clients[b].getNickName())) veryTempSocket = _clients[b].getSocketFd();
                        if (veryTempSocket) sendMessage(veryTempSocket, chanMsg);
                    }
                    break ;
                }
                if (a == MAXCHANS - 1) return (ERR_NOSUCHCHANNEL);
                continue ;
            }            
            if (!flag) return (ERR_NOSUCHNICK);
        }
    }
    return (MSG_OK);
}

std::string Server::handleJoinCommand(Client &c, char * join) {
    if (!c.getHasBeenWelcomed()) return (ERR_NOTREGISTRED);
    if (!join[0]) return (ERR_NEEDMOREPARAMS);
    if (join[0] == ' ') return (ERR_ERRONEOUSCHANNAME);
    if (join[0] != '&') return (ERR_ERRONEOUSCHANNAME);
    if (join[0] == '&' && join[1] && join[1] == ' ') return (ERR_ERRONEOUSCHANNAME);
    int i = 0;
    while (join[i]) {
        if (join[i] == ' ') break;
        if (join[i] == ',') return (ERR_ERRONEOUSCHANNAME);
        i++;
    }
    std::string tempJoin(join);
    std::string tempChanName = tempJoin.substr(0, tempJoin.find(' '));
    int a = chanExists(tempChanName);
    if (join[i] && join[i] == ' ' && join[i + 1] && _channels[a].getChanName()[0]) {                                             // if there is a character and the channel is valid (with an invitation key set) it means there is a channel password
        // std::string tempChanKey = tempJoin.substr(i + 1, tempJoin.length());
        return ("To be continued...");                                                          // joining an existent channel is handled in handleJoinCommandTwo
    }
    else if (join[i] && join[i] == ' ' && !join[i + 1]) return (ERR_ERRONEOUSCHANNAME);         // if there is a space but no characters it is an invalid channel name
    else {
        if (a == MAXCHANS - 1) return (ERR_NOSUCHCHANNEL);
        if (!stringCompareTheReturn(_channels[a].getChanName(), tempChanName)) {                // joining an existent channel
            if (_channels[a].isChanMember(c.getNickName())) return (ERR_ALREADYONCHAN);
            _channels[a].addChanMember(c.getNickName()); return (CHAN_JOINED + tempChanName);
        }
        else {createChan(tempChanName, c.getNickName(), a); return(CHAN_CREATED);}
    }
    return ("...");
}

int Server::chanExists(std::string chanName) {
    int a = getNewChanIndex();                  // a is the index of the new channel or the one that the client is trying to join
    if (!a) return (0);
    int i = 0;
    for (int i = 0; i < MAXCHANS; i++) {if (!stringCompareTheReturn(chanName, _channels[i].getChanName())) return (i);}
    return (a);
}

int Server::getNewChanIndex(void) {
    for (int i = 0; i < MAXCHANS; i++) {if (!_channels[i].getChanName()[0]) return (i);}            // gets the first free index inside _channels
    return (-1);            // -1 means MAXCHANS has been reached
}

void    Server::createChan(std::string chanName, std::string chanFounder, int a) {
    _channels[a].setChanName(chanName);
    _channels[a].addChanOp(chanFounder);
    _channels[a].addChanMember(chanFounder);
}

Server::~Server (void) {}