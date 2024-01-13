#include "../Includes/Utils.hpp"

void	handleSigInt(int signal) {std::cout<<std::endl; exit(0);}

void    std_errore(const char *err) {std::cerr<<"Error: "<<err<<std::endl; exit(1);}

int  parseInt(std::string str) {
	int	i = 0;
	int	ret = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)) i++;
	while (str[i]) {if (!isdigit(str[i])) return (-1); else ret = ret * 10 + str[i++] - '0';}
	return (ret);
}

int isAlpha(char c) {
    if (c >= 'a' && c <= 'z') return (0);
    else return (1);
}

int	checkMssgSyntax(std::string msg) {					// only checks for empty spaces between each word
	if (!msg[0]) return (1);                            // checks if the string exists
	if (isAlpha(msg[0]) && msg[0] != ':') return (1);
    if (msg.length() > 513) return (1);                 // maximum size is 512 + '\0'
    int i = 0;
	if (msg[i] == ' ') return (1);
    if (msg[i] == ':') {
		i++;
		while (msg[i] && msg[i] != ' ') i++;
	}
    if (msg[i] == ' ' && (msg[i + 1] && (msg[i + 1] == '\r' || msg[i + 1] == '\n'))) return (1);                    // checks for single words with a space messages
    if (msg[i + 1] && msg[i + 1] == ' ') return (1);                                                                // checks if there are two consecutive spaces
    return (0);
}

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
	while (second[i]) {if (second[i] == first[i]) i++; else return (1);}
	if (first[i] == '\n' || first[i] == '\r' || first[i] == '\0') return (0);
	return (1);
}