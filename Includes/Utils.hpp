#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

typedef struct	s_command
{
    std::string     prefix;
    std::string     command;
    std::string     parameters[10];
}				t_command;

void	handleSigInt(int signal);
void    std_errore(const char *err);
int     parseInt(std::string str);

#endif