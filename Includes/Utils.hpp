#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstring>

int     checkModeSyntaxOne(char * mode);
int     checkModeSyntaxTwo(char * mode, int *i);
int	    checkMssgSyntax(std::string msg);
void	handleSigInt(int signal);
int     isAlpha(char c);
int     isDigit(char c);
int     parseInt(std::string str);
void    std_errore(const char *err);
int	    stringCompare(char * first, std::string second);
int     stringCompareTheReturn(std::string first, std::string second);

#endif