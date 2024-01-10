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

int	stringCompare(std::string first, std::string second) {
	int	i = 0;
	if (!first[0] || !second[0]) return (1);
	while (second[i]) {if (second[i] == first[i]) i++; else return (1);}
	if (first[i] == '\n' || first[i] == '\r' || first[i] == '\0') return (0);
	return (1);
}

int checkCommandOrPrefix(std::string msg) {
    if (!stringCompare(msg.substr(0, msg.find(' ')), "pass")) return (1);	// pass command
    return (0);																// command not found
}