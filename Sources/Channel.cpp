#include "../Includes/Channel.hpp"
#include "../Includes/Utils.hpp"

Channel::Channel (void) {};

Channel::Channel (const Channel &chan) {*this = chan;}

Channel &Channel::operator=(const Channel &chan) {
    if (this == &chan) return (*this);
    *this = Channel(chan);
    return (*this);
}

std::string Channel::getChanName(void) const {return _chanName;}

void    Channel::setChanName(std::string chanName) {_chanName = chanName;}

int Channel::addChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {if (!_chanOps[i][0]) _chanOps[i] = nname; return (i);}
    return (-1);        // -1 means chanops limit has been reached
}

int Channel::isChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {if (!stringCompareTheReturn(nname, _chanOps[i])) return (1);}
    return (0);
}

Channel::~Channel () {};