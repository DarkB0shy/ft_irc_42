#include "../Includes/Channel.hpp"
#include "../Includes/Utils.hpp"

Channel::Channel (void) {_chanSize = 50; _inviteOnly = 0; _topicOpOnly = 1;};

Channel::Channel (const Channel &chan) {*this = chan;}

Channel &Channel::operator=(const Channel &chan) {
    if (this == &chan) return (*this);
    *this = Channel(chan);
    return (*this);
}

std::string Channel::getChanKey(void) const {return _chanKey;}

std::string Channel::getChanName(void) const {return _chanName;}

int Channel::getChanSize(void) const {return _chanSize;}

std::string Channel::getChanTopic(void) const {return _chanTopic;}

int Channel::getInviteOnly(void) const {return _inviteOnly;}

int Channel::getTopicOpOnly(void) const {return _topicOpOnly;}


void    Channel::setChanKey(std::string chanKey) {_chanKey = chanKey;}

void    Channel::setChanName(std::string chanName) {_chanName = chanName;}

void    Channel::setChanSize(int chanSize) {_chanSize = chanSize;}

void    Channel::setChanTopic(std::string chanTopic) {_chanTopic = chanTopic;}

void    Channel::setInviteOnly(int iOnly) {_inviteOnly = iOnly;}

void    Channel::setTopicOpOnly(int tOnly) {_topicOpOnly = tOnly;}

int Channel::addChanMember(std::string nname) {
    int i = 0;
    for (i = 0; i < _chanSize; i++) {
        if (!_chanMembers[i][0]) {_chanMembers[i] = nname; return (i);}
        continue;    
    }
    return (i);                                                                     // chanmembers limit reached
}

int Channel::addChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {
        if (!_chanOps[i][0]) {_chanOps[i] = nname; return (i);}
        continue;
    }
    return (-1);                                                                    // -1 means chanops limit has been reached
}

void    Channel::addNnameToInviteList(std::string nname) {
    for (int i = 0; i < MAX_CHANMEMBERS; i++) {
        if (!_inviteList[i][0]) _inviteList[i] = nname;
        continue ;
    }
}

void    Channel::emptyChan(void) {
    int i = 0;
    for (i = 0; i < _chanSize; i++) {
        if (!_chanMembers[i][0]) continue;
        else return ;
    }
    for (int a = 0; a < MAX_CHANMEMBERS; a++) _chanMembers[a] = {'\0'};
    for (int b = 0; b < MAX_CHANOPS; b++) _chanOps[b] = {'\0'};
    for (int c = 0; c < MAX_CHANMEMBERS; c++) _inviteList[c] = {'\0'};
    _chanName = {'\0'};
    _chanTopic = {'\0'};
    _chanKey = {'\0'};
    _inviteOnly = 0;
    _topicOpOnly = 0;
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
}

int Channel::isAlreadyInvited(std::string nname) {
    for (int i = 0; i < MAX_CHANMEMBERS; i++) {
        // std::cout<<_inviteList[i]<<std::endl;
        if (!stringCompareTheReturn(nname, _inviteList[i])) return (1);
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

int Channel::isChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {
        if (!stringCompareTheReturn(nname, _chanOps[i])) return (1);
        continue;
    }
    return (0);
}

void    Channel::removeChanMember(std::string nname) {
    for (int i = 0; i < _chanSize; i++) {
        if (!stringCompareTheReturn(nname, _chanMembers[i])) {_chanMembers[i] = {'\0'}; return ;}
        continue;
    }
}

void    Channel::removeChanOp(std::string nname) {
    for (int i = 0; i < MAX_CHANOPS; i++) {
        if (!stringCompareTheReturn(nname, _chanOps[i])) {_chanOps[i] = {'\0'}; return ;}
        continue;
    }
}

void    Channel::removeNnameFromInviteList(std::string nname) {
    for (int i = 0; i < MAX_CHANMEMBERS; i++) {
        if (!stringCompareTheReturn(nname, _inviteList[i])) {_inviteList[i] = {'\0'};}
        continue ;
    }
}

Channel::~Channel () {};