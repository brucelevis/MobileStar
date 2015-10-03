#define _CRT_SECURE_NO_WARNINGS

#include "Channel.h"
#include "Log.h"
#include "User.h"
#include "LobbyDefine.h"


Channel::Channel()
{

}

bool Channel::initialize(ChannelInfo* channelInfo)
{
    channelNo = channelInfo->channelNo;
    channelNameLen = channelInfo->channelNameLen;
    memset(channelName, 0, MAX_CHANNEL_NAME_LEN);
    memcpy(channelName, channelInfo->channelName, channelNameLen);
    return true;
}

int Channel::addUser(User* user)
{
	userList.push_back(user);
    
    user->setUserState(USER_STATE_LOBBY);
    user->setLoctionObject(USER_LOCATION_CHANNEL, (void*)this);

	return SUCCESS;
}

int Channel::removeUser(User* user)
{
	userList.remove(user);
    
    user->setUserState(USER_STATE_PROCESS_STATE);
    user->setLoctionObject(USER_LOCATION_NO, NULL);

	return SUCCESS;
}

Channel::~Channel()
{

}

//ChannelManager

ChannelManager::ChannelManager()
{
    createChannelNo = 1;
}

bool ChannelManager::initialize(int channelCount, ChannelInfo* channelInfo)
{
    channelList.clear();
    for (int i = 0; i < channelCount; i++)
    {
        channelInfo[i].channelNo = createChannelNo++;
        Channel* channel = new Channel();
        if(channel->initialize(&channelInfo[i]) == false)
        {
            ErrorLog("channel lnit error");
            return false;
        }
        
        channelList.push_back(channel);
    }
    
    return true;
}

int16_t ChannelManager::getRandomChannelNo()
{
	return channelList.at(0)->getChannelNo();
}

Channel* ChannelManager::getChannelByChannelNo(int16_t channelNo)
{
    Channel* channel = NULL;
    
    for (itr = channelList.begin(); itr != channelList.end(); itr++)
    {
        
        channel = *itr;
        if(channel->getChannelNo() == channelNo)
        {
            DebugLog("channel %d", channelNo);
            return channel;
        }
    }

	return NULL;
}

int16_t ChannelManager::firstEnterUser(User* user)
{
	int channelNo = getRandomChannelNo();
    
    Channel* channel = getChannelByChannelNo(channelNo);
	int failReason = channel->addUser(user);
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