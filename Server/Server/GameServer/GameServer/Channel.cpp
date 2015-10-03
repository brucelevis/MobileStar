#define _CRT_SECURE_NO_WARNINGS

#include "Channel.h"
#include "Log.h"
#include "User.h"
#include "GameDefine.h"
//#include "Session.h"

Channel::Channel()
{

}

bool Channel::Initialize(int channelNameInt)
{
	m_channelNo = 0;
	m_userList.clear();
	memset(m_channelName, 0, BasicPacket::MAX_CHANNEL_NAME_LEN + 1);
	sprintf(m_channelName, "%s%d", "channel", channelNameInt);
	return true;
}

bool Channel::InitChannel(ChannelInfo* channelInfo)
{
	m_channelNo = channelInfo->channelNo;
	memset(m_channelName, 0, BasicPacket::MAX_CHANNEL_NAME_LEN + 1);
	memcpy(m_channelName, channelInfo->channelName, BasicPacket::MAX_CHANNEL_NAME_LEN);
	return true;
}

int Channel::AddUser(User* user)
{
	m_userList.push_back(user);

	return SUCCESS;
}

int Channel::RemoveUser(User* user)
{
	m_userList.remove(user);

	return SUCCESS;
}

Channel::~Channel()
{

}

//ChannelManager

ChannelManager::ChannelManager()
{

}

bool ChannelManager::Initialize()
{
	for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		if (m_channelList[i].Initialize(i) == false)
		{
			ErrorLog("channel lnit error");
			return false;
		}
	}
	return true;
}

bool ChannelManager::InitChannel(int channelCount, ChannelInfo* channelInfo)
{
	for (int i = 0; i < channelCount; i++)
	{
		if (m_channelList[i].InitChannel(&channelInfo[i]) == false)
		{
			ErrorLog("channel lnit error");
			return false;
		}
	}

	return true;
}

int16_t ChannelManager::GetRandomChannelNo()
{
	return 0;
}

Channel* ChannelManager::GetChannelByChannelNo(int16_t channelNo)
{
	if (channelNo >= MAX_CHANNEL_COUNT)
	{
		return NULL;
	}

	return &m_channelList[channelNo];
}

int16_t ChannelManager::FirstEnterUser(User* user)
{
	int channelNo = GetRandomChannelNo();
	int failReason = m_channelList[channelNo].AddUser(user);
	if (failReason != SUCCESS)
	{
		ErrorLog("FirstEnterUser");
		return INVALID_CHANNEL_NO;
	}

	return channelNo;
}

ChannelManager::~ChannelManager()
{

}
/*
int ChannelManager::AddUser(Session* session, const char* nickName, int nickNameLen)
{
	User* user = new User(session);
	if (user->Initialize(m_createUserNo, nickName, nickNameLen) == false)
	{
		ErrorLog("user Initialize error");
		return INTERNAL_ERROR;
	}
	if (m_userMap.insert(boost::unordered_map<int64_t, User*>::value_type(user->m_userNo, user)).second == false)
	{
		ErrorLog("user insert fail");
		return INTERNAL_ERROR;
	}

	session->userKey = m_createUserNo++;

	return SUCCESS;
}

User* UserManager::GetUserByUserNo(int64_t userNo)
{
	boost::unordered_map< int64_t, User* >::const_iterator iter = m_userMap.find(userNo);

	if (iter == m_userMap.end())
	{
		ErrorLog("not exist User");
		return NULL;
	}

	return iter->second;
}

bool UserManager::RemoveUser(User* user)
{
	if( m_userMap.erase(user->m_userNo) != 1 )
	{
		ErrorLog("erase fail");
		return false;
	}

	user->m_session = NULL;

	delete ( user );

	return true;
}

bool UserManager::RemoveUserByUserNo(int64_t userNo)
{
	User* user = GetUserByUserNo(userNo);
	
	if(user == 0)
	{
		ErrorLog("user not exist");
		return false;
	}

	if( m_userMap.erase(userNo) != 1 )
	{
		ErrorLog("erase fail");
		return false;
	}

	delete ( user );

	return true;
}


*/