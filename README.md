<h1 align="center">ft_irc</h1>

<p align="right">Final score: 100</p>

This C++98 project is an Internet Relay Chat (or IRC) server. The latest up-to-date Request For Comments that describe this Internet standard are 2810, 2811, 2812 and 2813.
The main purpose of an IRC server is to do message relaying between clients connected to it. This means two clients that communicate are never connected directly: they talk to each other through at least one server.
This happens thanks to sockets. More on sockets in ./what_the_socket.txt. Every client is
a socket so this is a multi-thread application and operations are non blocking:

fcntl(_socket, F_SETFL, O_NONBLOCK) // ./Sources/Server.cpp line 24

Clients can both talk to each other directly and through channels. A Channel is a "group" with a common topic of
online registered users; there are also super-users (chan-ops) that can moderate the channel. There are at least 4 types of IRC channels, but this is just one local server, so only local channels are implemented. Their name must start with {&} as according to IRC-CHAN.

To use this program just run the Makefile, launch the server and connect to it with a TCP client like Netcat this way:

$make

$./ircserv 6667 pswd

Then open a new terminal and connect to your localhost:

$nc localhost 6667

In order to register and use all of the available commands, any client has to first guess the password, set a nickname and a username. Every command demanded by the subject is as loyal as possible to the definition
held in the RFC's:

RFC 2810 : holds a general description of what an Internet Relay Chat Server does together
		with useful client-server concepts.

RFC 2811 : IRC-CHAN. This document describes how channels should be implemented and talks about
		channel modes in detail.

RFC 2812 : Client Protocol. Explains the general syntax of any IRC message, how to register to 
		the server and most of the commands available to clients.

RFC 2813 : Server Protocol. This document explains how server-server communication should work.

[alpelliz](https://github.com/Beta-J23) helped me trouble shooting some of the commands.
