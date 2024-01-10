#include "Includes/Server.hpp"

int main (int argc, char **argv) {
	if (argc != 3 ) std_errore(WRONGARGS);
    int portNum = parseInt(argv[1]);
	if (portNum <= 1024 || portNum > 65535) std_errore(WRONGPORT);
    Server newServer(portNum, argv[2]);
    signal(SIGINT, handleSigInt);
    newServer.startServer();
    while (1) newServer.runServer();
    newServer.clearSocketsSet();
    return(0);
}