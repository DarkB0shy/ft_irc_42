#include "../includes/Server.hpp"

Server::Server (void) {};

Server::Server (int portNumber, std::string pass) {
    _portNumber = portNumber;
    _pass = pass;
}

Server::Server (const Server &s) {*this = s;}

Server &Server::operator=(const Server &s) {
    if (this == &s)
        return (*this);
    *this = Server(s);
    return (*this);
}

int Server::getSocket() const { return (_socket); }

void	Server::sendMessage(const int socket, const std::string &message) {
    int send_res = send(socket, message.c_str(), message.size(), MSG_NOSIGNAL);
    if (send_res == -1) {
        switch (errno) {
            case EPIPE:
                return ;
            default:
                std_errore(OUTERR);
        }
    }
}

void    Server::initClients(void) {
    for (int i = 0; i < MAXCLIENTS; i++)
        _clients[i].setSocketFd(0);
}

void    Server::startServer(void) {
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        std_errore(NOSOCKET);
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
        std_errore(NOSOCKETFLAGS);
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;                          // this allows connections from every network interface available
    _address.sin_port = htons(_portNumber);
    _addrlen = sizeof(_address);
    if (bind(_socket, (struct sockaddr *)&_address, _addrlen) < 0)
        std_errore(PORTNOTBINDED);
    if (listen(_socket, MAXCONNECTIONS) < 0)
        std_errore(NOTLISTENING);
    initClients();
    std::cout<<WELCOMETOSERVER<<_portNumber<<std::endl;
}

void    Server::clearSocketsSet (void) {
	for (int fd = 0; fd <= _maxFd; fd++)
		if (FD_ISSET(fd, &_readFds))
			close(fd);
	close(this->getSocket());
}

void    Server::resetSocketSet (void) {
    int clientFd;
    // Clears socket set
    FD_ZERO(&_readFds);
    // Adds server socket to the set
    FD_SET(this->getSocket(), &_readFds);
    _maxFd = this->getSocket();
    // Adds the other sockets to the set
    for (int i = 0; i < MAXCLIENTS; i++) {
        clientFd = _clients[i].getSocketFd();
        // If its a valid fd it is added to the set
        if (clientFd > 0)
            FD_SET(clientFd, &_readFds);
        // The highest Fd is needed for select()
        if (clientFd > _maxFd)
            _maxFd = clientFd;
    }
}

void    Server::handleNewConnection(void) {
    t_socket    tempSocket;
	tempSocket.addrlen = sizeof(tempSocket.address);
	if ((tempSocket.socket = accept(this->getSocket(), (struct sockaddr *)&tempSocket.address, (socklen_t*)&tempSocket.addrlen)) < 0)
        std_errore(NEWCONNERR);
	std::cout<<CONNHANDLED<<inet_ntoa(tempSocket.address.sin_addr)<<", "<<ntohs(tempSocket.address.sin_port)<<std::endl;
    // Adds the temp socket to this->_clients
	for (int i = 0; i < MAXCLIENTS; i++) {
		if( _clients[i].getSocketFd() == 0 ) {
			_clients[i].setSocketFd(tempSocket.socket);
			_clients[i].setIpAddress(inet_ntoa(tempSocket.address.sin_addr));
			_clients[i].setPort(ntohs(tempSocket.address.sin_port));
            break;
		}
	}
}

void    Server::handleClientInput(Client &c) {
    char    buffa[BUFFASIZE];

    int valread = recv(c.getSocketFd(), buffa, BUFFASIZE - 1, 0);
    if (valread == -1)
        std_errore(READERR);
    else if (valread == 0) {
        std::cout<<CLOSEDCONN<<c.getIpAddress()<<", "<<c.getPort()<<std::endl;
        c.setSocketFd(0);
        return ;
    }
    else {
        buffa[valread] = '\0';
        std::cout<<buffa;
        sendMessage(c.getSocketFd(), "Sup\r\n");
    }
}

void    Server::runServer(void) {
    resetSocketSet();
    // Maxfd is added to the current set of file descriptors
    // Fd sets can handle multiple file descriptors 
	if (select(_maxFd + 1, &_readFds, NULL, NULL, NULL) < 0)
		std_errore(FDSETERROR);
    if (FD_ISSET(this->getSocket(), &_readFds))
        handleNewConnection();
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (FD_ISSET(_clients[i].getSocketFd(), &_readFds))
            handleClientInput(_clients[i]);
    }
}

Server::~Server (void) {}