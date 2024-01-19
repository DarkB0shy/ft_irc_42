#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define MAX_CHANMEMBERS 50
#define MAX_CHANOPS 50

#include <string>

class Channel {
    private:
        std::string _chanName;
        std::string _chanOps[MAX_CHANOPS];
        std::string _chanMembers[MAX_CHANMEMBERS];
        std::string _inviteList[MAX_CHANMEMBERS];   // used for handling invite-only mode
        std::string _chanTopic;
        std::string _chanKey;
        int         _chanSize;
        int         _inviteOnly;                    // 0 means channel is open, 1 means only on invitation
        int         _topicOpOnly;                   // 0 means topic command can be used by all members, 1 only by chanops
    public:
        std::string getChanName(void) const;
        std::string getChanTopic(void) const;
        std::string getChanKey(void) const;
        int         getChanSize(void) const;
        int         getInviteOnly(void) const;
        int         getTopicOpOnly(void) const;
        void        setChanName(std::string chanName);
        void        setChanTopic(std::string chanTopic);
        void        setChanKey(std::string chanKey);
        void        setChanSize(int chanSize);
        void        setInviteOnly(int iOnly);
        void        setTopicOpOnly(int tOnly);
        int         addChanOp(std::string nname);       // returns 0 if MAX_CHANOPS has been reached
        int         addChanMember(std::string nname);
        void        removeChanOp(std::string nname);
        void        removeChanMember(std::string nname);
        int         isChanOp(std::string nname);        // returns 1 if nname is inside _chanOps
        int         isChanMember(std::string nname);
        void        emptyChan(void);                    // checks if the channel is empty once a user left
        int         fullChan(void);
        void        addNnameToInviteList(std::string nname);
        void        removeNnameFromInviteList(std::string nname);
        Channel(void);
        Channel(const Channel &chan);
        Channel &operator=(const Channel &chan);
        ~Channel();
};

#endif