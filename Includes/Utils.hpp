#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstring>

void	handleSigInt(int signal);
void    std_errore(const char *err);
int     parseInt(std::string str);
int     isAlpha(char c);
int	    checkMssgSyntax(std::string msg);
int	    stringCompare(char * first, std::string second);
int     stringCompareTheReturn(std::string first, std::string second);

#endif