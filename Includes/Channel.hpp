#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define MAX_CHANMODES 6
#define MAX_CHANMEMBERS 50
#define MAX_CHANOPS 50

#include <string>

class Channel {
    private:
        std::string _chanName;
        std::string _chanOps[MAX_CHANOPS];
        std::string _chanModes[MAX_CHANMODES];
        std::string _chanMembers[MAX_CHANMEMBERS];
    public:
        std::string getChanName(void) const;
        void        setChanName(std::string);
        int         addChanOp(std::string nname);       // returns 0 if MAX_CHANOPS has been reached
        int         addChanMember(std::string nname);
        int         isChanOp(std::string nname);        // returns 1 if nname is inside _chanOps
        int         isChanMember(std::string name);
        Channel(void);
        Channel(const Channel &chan);
        Channel &operator=(const Channel &chan);
        ~Channel();
};

#endif