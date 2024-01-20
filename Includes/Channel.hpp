#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define MAX_CHANMEMBERS 50
#define MAX_CHANOPS 50

#include <string>

class Channel {
    private:
        std::string _chanKey;
        std::string _chanMembers[MAX_CHANMEMBERS];
        std::string _chanName;
        std::string _chanOps[MAX_CHANOPS];
        int         _chanSize;
        std::string _chanTopic;
        std::string _inviteList[MAX_CHANMEMBERS];                   // used for handling invite-only mode
        int         _inviteOnly;                                    // 0 means channel is open, 1 means only on invitation
        int         _topicOpOnly;                                   // 0 means topic command can be used by all members, 1 only by chanops
    public:
        Channel(void);
        Channel(const Channel &chan);
        Channel &operator=(const Channel &chan);
        std::string getChanKey(void) const;                         // get methods
        std::string getChanName(void) const;
        int         getChanSize(void) const;
        std::string getChanTopic(void) const;
        int         getInviteOnly(void) const;
        int         getTopicOpOnly(void) const;
        void        setChanKey(std::string chanKey);                // set methods
        void        setChanName(std::string chanName);
        void        setChanSize(int chanSize);
        void        setChanTopic(std::string chanTopic);
        void        setInviteOnly(int iOnly);
        void        setTopicOpOnly(int tOnly);
        int         addChanMember(std::string nname);               // returns _chanSize if channel is full
        int         addChanOp(std::string nname);
        void        addNnameToInviteList(std::string nname);        // used for handling invite command
        void        emptyChan(void);                                // checks if the channel is empty once a user left
        int         fullChan(void);                                 // returns 1 if channel is full
        int         isAlreadyInvited(std::string nname);
        int         isChanMember(std::string nname);                // returns 1 if nname is inside _chanMembers
        int         isChanOp(std::string nname);
        void        removeChanMember(std::string nname);
        void        removeChanOp(std::string nname);
        void        removeNnameFromInviteList(std::string nname);
        ~Channel();
};

#endif