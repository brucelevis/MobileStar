#define _CRT_SECURE_NO_WARNINGS

#include "Channel.h"
#include "Log.h"
#include "User.h"
#include "LobbyDefine.h"
#include "LobbyServer.h"
#include "Network.h"
#include "ClientLobbyPacket.h"


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
    //////////////// notify to other users
    
    ClientLobbyPacket::EnterUserInChannelNotifyPacket notifyPacket;
    
    notifyPacket.userViewInfo.userNo = user->getUserNo();
    notifyPacket.userViewInfo.nickNameInfo.nickNameLen = user->getNickNameLen();
    memcpy(notifyPacket.userViewInfo.nickNameInfo.nickName, user->getNickName(), user->getNickNameLen());
    
    
    char* pNotifyBuffer = notifyBuffer;
    
    memcpy(pNotifyBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pNotifyBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pNotifyBuffer, &notifyPacket.userViewInfo.userNo, sizeof(notifyPacket.userViewInfo.userNo));
    pNotifyBuffer += sizeof(notifyPacket.userViewInfo.userNo);
    
    memcpy(pNotifyBuffer, &notifyPacket.userViewInfo.nickNameInfo.nickNameLen, sizeof(notifyPacket.userViewInfo.nickNameInfo.nickNameLen));
    pNotifyBuffer += sizeof(notifyPacket.userViewInfo.nickNameInfo.nickNameLen);
    
    memcpy(pNotifyBuffer, &notifyPacket.userViewInfo.nickNameInfo.nickName, notifyPacket.userViewInfo.nickNameInfo.nickNameLen);
    pNotifyBuffer += notifyPacket.userViewInfo.nickNameInfo.nickNameLen;
    
    
    
    
    std::list<User*>::iterator itr;
    
    User* otherUser = NULL;
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        otherUser = *itr;
        
        LobbyServer::getInstance()->network->sendPacket(otherUser->getConnectInfo(), (char*)&notifyPacket, sizeof(notifyPacket));
    }
    
    
    
    
    
    //////////////////////////////////////// add user

    
    
	userList.push_back(user);
    
    user->setUserState(USER_STATE_LOBBY);
    user->setLoctionObject(USER_LOCATION_CHANNEL, (void*)this);
    
    
    
    
    //////////////////////////////// send userList
    
    
    ClientLobbyPacket::ChannelInfoNotifyPacket packet;
    
    packet.channelNo = channelNo;
    packet.channelNameLen = channelNameLen;
    memcpy(packet.channelName, channelName, channelNameLen);

    int i = 0;
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        User* otherUser = *itr;
        packet.userViewInfoList[i].userNo = otherUser->getUserNo();
        packet.userViewInfoList[i].nickNameInfo.nickNameLen = otherUser->getNickNameLen();
        memcpy(packet.userViewInfoList[i].nickNameInfo.nickName, otherUser->getNickName(), otherUser->getNickNameLen());
        
        i++;
    }
    
    packet.userCount = i;
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);
    memcpy(pSendBuffer, &packet.channelNo, sizeof(packet.channelNo));
    pSendBuffer += sizeof(packet.channelNo);
    memcpy(pSendBuffer, &packet.channelNameLen, sizeof(packet.channelNameLen));
    pSendBuffer += sizeof(packet.channelNameLen);
    memcpy(pSendBuffer, packet.channelName, packet.channelNameLen);
    pSendBuffer += packet.channelNameLen;
    
    
    
    memcpy(pSendBuffer, &packet.userCount, sizeof(packet.userCount));
    pSendBuffer += sizeof(packet.userCount);
    
    for(i = 0; i < packet.userCount; i++)
    {
        memcpy(pSendBuffer, &packet.userViewInfoList[i].userNo, sizeof(packet.userViewInfoList[i].userNo));
        pSendBuffer += sizeof(packet.userViewInfoList[i].userNo);
        
        memcpy(pSendBuffer, &packet.userViewInfoList[i].nickNameInfo.nickNameLen, sizeof(packet.userViewInfoList[i].nickNameInfo.nickNameLen));
        pSendBuffer += sizeof(packet.userViewInfoList[i].nickNameInfo.nickNameLen);
        
        memcpy(pSendBuffer, packet.userViewInfoList[i].nickNameInfo.nickName, packet.userViewInfoList[i].nickNameInfo.nickNameLen);
        pSendBuffer += packet.userViewInfoList[i].nickNameInfo.nickNameLen;
    }
    
    LobbyServer::getInstance()->network->sendPacket(user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));


	return SUCCESS;
}

int Channel::removeUser(User* user)
{
	userList.remove(user);
    
    user->setUserState(USER_STATE_PROCESS_STATE);
    user->setLoctionObject(USER_LOCATION_NO, NULL);

    
    //////////////// notify to other users
    
    ClientLobbyPacket::LeaveUserInChannelNotifyPacket notifyPacket;
    
    notifyPacket.userNo = user->getUserNo();
    
    std::list<User*>::iterator itr;
    
    User* otherUser = NULL;
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        otherUser = *itr;
        
        LobbyServer::getInstance()->network->sendPacket(otherUser->getConnectInfo(), (char*)&notifyPacket, sizeof(notifyPacket));
    }
    
    ////////////////////////////////////////
    
    
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