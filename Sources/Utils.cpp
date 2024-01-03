#include "../Includes/Utils.hpp"

void	handleSigInt(int signal) {exit(0);}

void    std_errore(const char *err) {
    std::cerr<<"Error: ";
    std::cerr<<err<<std::endl;
    exit(1);
}

int  parseInt(std::string str) {
	int	i = 0;
	int	ret;

	while (str[i]==32 || (str[i] >= 9 && str[i] <= 13)) i++;
	ret = 0;
	while (str[i])
	{
		if (!isdigit(str[i])) return (-1);
		ret = ret * 10 + str[i++] - '0';
	}
	return (ret);
}