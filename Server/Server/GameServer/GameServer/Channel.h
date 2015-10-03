
// create by yong chan kim
// since 2014.7.29


#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <stdint.h>
#include <list>
#include "BasicPacket.h"

const int MAX_CHANNEL_COUNT = 3;

struct ChannelInfo
{
	int8_t channelNo;
	char channelName[BasicPacket::MAX_CHANNEL_NAME_LEN + 1];
};

class User;
class UserInfo;
class Session;

class Channel
{
public:

	Channel();

	bool Initialize(int channelNameInt);
	bool InitChannel(ChannelInfo* channelInfo);
	int AddUser(User* user);
	int RemoveUser(User* user);
	int16_t GetChannelNo() { return m_channelNo; }
	const char* GetChannelName() { return m_channelName; }
	const std::list<User*> GetUserList() { return m_userList; }
	~Channel();

public:

	friend class ChannelManager;

private:
	int16_t m_channelNo;
	char m_channelName[BasicPacket::MAX_CHANNEL_NAME_LEN + 1];
	std::list<User*> m_userList;
};


class ChannelManager
{
public:
	ChannelManager();
	bool Initialize();
	bool InitChannel(int channelCount, ChannelInfo* channelInfo);
	Channel* GetChannelByChannelNo(int16_t channelNo);

	int16_t GetRandomChannelNo();
	int16_t FirstEnterUser(User* user);

	~ChannelManager();

private:
	Channel m_channelList[MAX_CHANNEL_COUNT];
};

#endif //__CHANNEL_H__
