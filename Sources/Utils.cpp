#include "../Includes/Utils.hpp"

int checkModeSyntaxOne(char * mode) {
    if (!mode[0]) return (1);                                                                           // ERR_NEEDMOREPARAMS
    if (mode[0] != '&') return (1);
    if (mode[0] == '&' && mode[1] && mode[1] == ' ') return (1);
    int i = 0;
    int returnInt = 0;
    while (mode[i]) {
        if (mode[i] == ' ') break ;
        i++;
    }
    if (!mode[i]) return (2);                                                                           // ERR_NOCHANMODES
    if (mode[i] && mode[i] == ' ' && !mode[i + 1]) return (2);
    if (mode[i] == ' ' && (mode[i + 1] == '-' || mode[i + 1] == '+')) {
        i++;
        if (!mode[i + 1]) return (2);
        else {
            i++;
            if (mode[i] == 'o' || mode[i] == 'k') {
                returnInt = checkModeSyntaxTwo(mode, &i);
            }
            else if (mode[i] == 'i' || mode[i] == 't') {
                if (mode[i + 1]) return (3);                                                            // ERR_TOOMANYPARAMETERS
            }
            else if (mode[i] == 'l') {
                if (mode[i - 1] == '-') {
                    if (mode[i + 1]) return (3);
                    else return (0);
                }
                else if (mode[i + 1] && mode[i + 1] == ' ' && mode[i + 2] && isDigit(mode[i + 2])) {
                    i = i + 2;
                    while (mode[i]) {
                        if (!isDigit(mode[i])) return (3);
                        if (mode[i] == ' ') return (3);
                        i++;
                    }
                }
                else return (1);
            }
            else return (2);
        }
    } else return (2);
    return (returnInt);
}

int checkModeSyntaxTwo(char * mode, int *i) {                                                           // checks for modes that accept a maximum of 3 parametrs (o, k)
    (*i)++;
    if (!mode[(*i)]) return (1);
    else {
        if (mode[(*i)] == ' ') {
            if (!mode[(*i) + 1]) return (1);
            else {                                                                                      // just checks if there are more than 3 "valid" nicknames for mode <channel> o [<nicknames>]
                (*i)++;
                int tooManyParams = 0;
                while (mode[(*i)]) {
                    if (mode[(*i)] == ' ') tooManyParams++;
                    (*i)++;
                }
                if (tooManyParams > 2) return (3);
            }
        }
    }
    return (0);
}

int	checkMssgSyntax(std::string msg) {					                                                // only checks for empty spaces between each word
	if (!msg[0]) return (1);                                                                            // checks if the string exists
	if (!isAlpha(msg[0]) && msg[0] != ':') return (1);
    if (msg.length() > 513) return (1);                                                                 // maximum size is 512 + '\0'
    int i = 0;
	if (msg[i] == ' ') return (1);
    if (msg[i] == ':') {
		i++;
		while (msg[i] && msg[i] != ' ') i++;
	}
    if (msg[i] == ' ' && (msg[i + 1] && (msg[i + 1] == '\r' || msg[i + 1] == '\n'))) return (1);        // checks for single words with a space messages
    if (msg[i + 1] && msg[i + 1] == ' ') return (1);                                                    // checks if there are two consecutive spaces
    return (0);
}

void	handleSigInt(int signal) {std::cout<<std::endl; exit(0);}

int isAlpha(char c) {
    if (c >= 'a' && c <= 'z') return (1);
    else return (0);
}

int isDigit(char c) {
    if (c >= '0' && c <= '9') return (1);
    return (0);
}

int  parseInt(std::string str) {
	int	i = 0;
	int	ret = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)) i++;
	while (str[i]) {if (!isdigit(str[i])) return (-1); else ret = ret * 10 + str[i++] - '0';}
	return (ret);
}

void    std_errore(const char *err) {std::cerr<<"Error: "<<err<<std::endl; exit(1);}

int	stringCompare(char * first, std::string second) {
	int	i = 0;
	if (!first[0] || !second[0]) return (1);
	while (second[i]) {if (second[i] == first[i]) i++; else return (1);}
	if (first[i] == '\n' || first[i] == '\r' || first[i] == '\0') return (0);
	return (1);
}

int	stringCompareTheReturn(std::string first, std::string second) {
	int	i = 0;
	if (!first[0] || !second[0]) return (1);
	while (second[i]) {
        if (!first[i]) return (1);
        if (second[i] == first[i]) i++; else return (1);
        }
	if (first[i] == '\n' || first[i] == '\r' || first[i] == '\0') return (0);
	return (1);
}