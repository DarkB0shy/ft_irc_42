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

std::string Channel::getChanTopic(void) const {return _chanTopic;}

std::string Channel::getChanKey(void) const {return _chanKey;}

int Channel::getChanSize(void) const {return _chanSize;}

void    Channel::setChanName(std::string chanName) {_chanName = chanName;}

void    Channel::setChanTopic(std::string chanTopic) {_chanTopic = chanTopic;}

void    Channel::setChanKey(std::string chanKey) {_chanKey = chanKey;}

void    Channel::setChanSize(int chanSize) {_chanSize = chanSize;}

int Channel::isChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {
        if (!stringCompareTheReturn(nname, _chanOps[i])) return (1);
        continue;
    }
    return (0);
}

int Channel::isChanMember(std::string nname) {
    for (int i = 0; i < MAX_CHANMEMBERS; i++) {
        if (!stringCompareTheReturn(nname, _chanMembers[i])) return (1);
        continue;
    }
    return (0);
}

int Channel::addChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {
        if (!_chanOps[i][0]) {_chanOps[i] = nname; return (i);}
        continue;
    }
    return (-1);        // -1 means chanops limit has been reached
}

int Channel::addChanMember(std::string nname) {
    int i = 0;
    for (i = 0; i < _chanSize; i++) {
        if (!_chanMembers[i][0]) {_chanMembers[i] = nname; return (i);}
        continue;    
    }
    return (i);        // chanmembers limit reached
}


int Channel::addChanMode(std::string chanMode) {
    for (int i = 0; i < MAX_CHANMODES; i++) {
        if (!_chanModes[i][0]) {_chanModes[i] = chanMode; return (i);}
        continue;
    }
    return (-1);
}

void    Channel::removeChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {
        if (!stringCompareTheReturn(nname, _chanOps[i])) {_chanOps[i] = {'\0'}; return ;}
        continue;
    }
}

void    Channel::removeChanMember(std::string nname) {
    for (int i = 0; i < _chanSize; i++) {
        if (!stringCompareTheReturn(nname, _chanMembers[i])) {_chanMembers[i].clear(); return ;}
        continue;
    }
}

void    Channel::removeChanMode(std::string chanMode) {
    for (int i = 0; i < MAX_CHANMODES; i++) {
        if (!stringCompareTheReturn(chanMode, _chanModes[i])) {_chanModes[i] = {'\0'}; return ;}
        continue ;
    }
}

void    Channel::emptyChan(void) {
    int i = 0;
    for (i = 0; i < _chanSize; i++) {
        if (!_chanMembers[i][0]) continue;
        else return ;
    }
    for (int a = 0; a < _chanSize; a++) _chanMembers[i] = {'\0'};
    _chanName = {'\0'};
    _chanTopic = {'\0'};
    _chanKey = {'\0'};
}

int Channel::fullChan(void) {
    if (_chanSize == 1) return (1);
    int i = 0;
    int check = 0;
    while (i < _chanSize) {
        if (_chanMembers[i][0]) check++;
        i++;
    }
    if (check == _chanSize) return (1);
    return (0);
    // if (_chanMembers[_chanSize - 1][0]) return (1);                             // cosi funziona piu o meno
    // return (0);

    // std::cout<<_chanMembers[_chanSize];

    // if (!stringCompareTheReturn(_chanMembers[_chanSize], _chanTopic)) {          // per qualche motivo il topic del canale finisce dentro i chan members?
    //     return (1);
    // }
    // return (0);
}

Channel::~Channel () {};