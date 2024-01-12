#include "../Includes/Message.hpp"

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
    } else return (NULL);
    if (mssg[i] || mssg[i] == ' ') {
        if (mssg[i] == ' ') i++;
        while (mssg[i]) {
            if (mssg[i] == '\0') break;
            tempParameters[j] = mssg[i];
            i++;
            j++;
        }
        tempParameters[j] = '\0';
        strcpy(tmpmssg->getParameters(), tempParameters);
    } else return (NULL);
    return (tmpmssg);
}

int	checkMssgSyntax(std::string msg) {					// only checks for empty spaces between each word
	if (msg.length() > 513) return (1);                 // maximum size is 512 + '\0'
    int i = 0;
	if (!msg[i]) return (1);                            // checks if the string exists
	if (msg[i] == ' ') return (1);
    if (msg[i] == ':') {
		i++;
		while (msg[i] && msg[i] != ' ') i++;
	}
	if (!msg[i] || (msg[i] == ' ' && msg[i + 1] && msg[i + 1] == ' ')) return (1);           // checks if the mssg is made up of one word
    while (msg[i] && msg[i] != ' ') {
        if (msg[i] == '\r' || msg[i] == '\n') return (1);                                    // checks for single words messages
        i++;
    }
    if (msg[i] == ' ' && (msg[i + 1] && (msg[i + 1] == '\r' || msg[i + 1] == '\n'))) return (1);                    // checks for single words with a space messages
    if (msg[i + 1] && msg[i + 1] == ' ') return (1);                                                                // checks if there are two consecutive spaces
    return (0);
}

Message::~Message(void) {}