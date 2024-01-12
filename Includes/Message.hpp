#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <cstring>

class Message {
    private:
        char _prefix[15];
        char _command[15];
        char _parameters[482];
    public:
        char*       getPrefix(void);
        char*       getCommand(void);
        char*       getParameters(void);
        Message*    splittedMssg(std::string mssg);
        Message(void);
        Message(const Message &m);
        Message &operator=(const Message &m);
        ~Message();
};

int     checkMssgSyntax(std::string msg);

#endif