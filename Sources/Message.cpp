#include "../Includes/Message.hpp"
#include "../Includes/Utils.hpp"

Message::Message (void) {}

Message::Message (const Message &m) {*this = m;}

Message &Message::operator=(const Message &m) {
    if (this == &m) return (*this);
    *this = Message(m);
    return (*this);
}

char*   Message::getPrefix(void) {return _prefix;}

char*   Message::getCommand(void) {return _command;}

char*   Message::getParameters(void) {return _parameters;}

Message*    Message::splittedMssg(std::string mssg) {                        // &mssg[n], n is the starting point of mssg and is also a char *
    if (checkMssgSyntax(mssg)) return NULL;
    Message *tmpmssg;
    tmpmssg = new Message();
    char tempPrefix[15];
    char tempCommand[15];
    char tempParameters[482];
    int i = 0;
    int j = 0;
    if (mssg[i] == ':') {
        i++;
        while (mssg[i] && mssg[i] != ' ') {
            tempPrefix[j] = mssg[i];
            i++;
            j++;
        }
        tempPrefix[j] = '\0';
        strcpy(tmpmssg->getPrefix(), tempPrefix);
        j = 0;
    }
    if (mssg[i] || mssg[i] == ' ') {
        if (mssg[i] == ' ') i++;
        while (mssg[i] && mssg[i] != ' ') {
            tempCommand[j] = mssg[i];
            i++;
            j++;
        }
        tempCommand[j] = '\0';
        strcpy(tmpmssg->getCommand(), tempCommand);
        j = 0;
    }
    if (mssg[i] || mssg[i] == ' ') {
        if (mssg[i] == ' ') i++;
        while (mssg[i]/* && mssg[i] != ' '*/) {
            if (mssg[i] == '\0' || mssg[i] == '\r' || mssg[i] == '\n') break;
            tempParameters[j] = mssg[i];
            i++;
            j++;
        }
        tempParameters[j] = '\0';
        strcpy(tmpmssg->getParameters(), tempParameters);
    }
    return (tmpmssg);
}

Message::~Message(void) {}

// int main (int argc, char **argv) {
//     char buffa[1025] = "user fred";
//     std::string n = "gio";
//     Message *newMssg;
//     newMssg = newMssg->splittedMssg(buffa);
//     std::cout<<newMssg->getParameters();
//     std::cout<<"del";
//     return(0);
// }