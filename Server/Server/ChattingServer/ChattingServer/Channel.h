
// create by yong chan kim
// since 2014.7.29


#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <stdint.h>
#include <list>
#include <vector>
#include "BasicPacket.h"


class User;
class UserInfo;

class Channel
{
public:

	Channel();

	bool initialize(ChannelInfo* channelInfo);
	int addUser(User* user);
	int removeUser(User* user);
	int16_t getChannelNo() { return channelNo; }
    int getChannelNameLen() { return channelNameLen; }
	const char* getChannelName() { return channelName; }
	std::list<User*> getUserList() { return userList; }
    bool isPossibleMove() { return (userList.size() < 200); }
	~Channel();

public:

	friend class ChannelManager;

private:
	int16_t channelNo;
    int channelNameLen;
	char channelName[MAX_CHANNEL_NAME_LEN + 1];
	std::list<User*> userList;
    
    char notifyBuffer[5000];
    char sendBuffer[50000];
};


class ChannelManager
{
public:
	ChannelManager();
	bool initialize(int channelCount, ChannelInfo* channelInfo);
    int createChannel(ChannelInfo* channelInfo);
    int deleteChannel(int16_t channelNo);
    
	Channel* getChannelByChannelNo(int16_t channelNo);

	int16_t getRandomChannelNo();
	int firstEnterUser(User* user);
    Channel* getChannelByIndex(int index) { return channelList.at(index); }
    int getChannelCount() { return (int)channelList.size(); }

	~ChannelManager();

private:
    std::vector<Channel*>::iterator itr;
    std::vector<Channel*> channelList;
    int commonChannelCount;
    unsigned long createChannelNo;
};

#endif //__CHANNEL_H__
