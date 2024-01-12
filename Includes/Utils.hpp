#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstring>

void	handleSigInt(int signal);
void    std_errore(const char *err);
int     parseInt(std::string str);
int	    stringCompare(std::string first, std::string second);
// int     checkCommandAndPrefix(std::string msg, std::string nname);

#endif