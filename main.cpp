#include "includes/Server.hpp"

void    std_errore(const char *err) {
    std::cerr<<ERROR;
    std::cerr<<err<<std::endl;
    exit(1);
}

static void	handleSigInt(int signal) {exit(0);}

int main (int argc, char **argv) {
	if (argc != 3 ) {std_errore(WRONGARGS);}
    int portNum = std::atoi(argv[1]);
    Server newServer(portNum, argv[2]);
    signal(SIGINT, handleSigInt);
    newServer.startServer();
    while (1)
        newServer.runServer();
    newServer.clearSocketsSet();
    return(0);
}