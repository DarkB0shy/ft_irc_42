#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <cstring>

class Message {
    private:
        char _command[15];
        char _parameters[482];
        char _prefix[15];
    public:
        Message(void);
        Message(const Message &m);
        Message &operator=(const Message &m);
        char*       getCommand(void);                   // get methods
        char*       getParameters(void);
        char*       getPrefix(void);
        Message*    splittedMssg(std::string mssg);     // splits std::string mssg into prefix, command and parameters (char *)
        ~Message();
};

#endif