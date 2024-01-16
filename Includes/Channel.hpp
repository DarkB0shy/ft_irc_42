#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define MAX_CHANMODES 6
#define MAX_CHANMEMBERS 2
#define MAX_CHANOPS 50

#include <string>

class Channel {
    private:
        std::string _chanName;
        std::string _chanOps[MAX_CHANOPS];
        std::string _chanModes[MAX_CHANMODES];
        std::string _chanMembers[MAX_CHANMEMBERS];
        std::string _chanTopic;
        std::string _chanKey;
        int         _chanSize = MAX_CHANMEMBERS;
    public:
        std::string getChanName(void) const;
        std::string getChanTopic(void) const;
        std::string getChanKey(void) const;
        int         getChanSize(void) const;
        void        setChanName(std::string chanName);
        void        setChanTopic(std::string chanTopic);
        void        setChanKey(std::string chanKey);
        void        setChanSize(int chanSize);
        int         addChanOp(std::string nname);       // returns 0 if MAX_CHANOPS has been reached
        int         addChanMember(std::string nname);
        int         addChanMode(std::string chanMode);
        void        removeChanOp(std::string nname);
        void        removeChanMember(std::string nname);
        void        removeChanMode(std::string chanMode);
        int         isChanOp(std::string nname);        // returns 1 if nname is inside _chanOps
        int         isChanMember(std::string name);
        void        emptyChan(void);                    // checks if the channel is empty once a user left
        int         fullChan(void);
        Channel(void);
        Channel(const Channel &chan);
        Channel &operator=(const Channel &chan);
        ~Channel();
};

#endif