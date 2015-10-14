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
    if(userList.size() >= 200)
    {
        DebugLog("user 200");
        return FULL_USER_IN_CHANNEL;
    }
    
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
    createChannelNo = 0;
}

bool ChannelManager::initialize(int channelCount, ChannelInfo* channelInfoList)
{
    channelList.clear();
    
    for(int i = 0; i < channelCount; i++)
    {
        if(createChannel(&channelInfoList[i]) < 0)
        {
            ErrorLog("channel init error");
            return false;
        }
    }
    
    commonChannelCount = channelCount;
    
    return true;
}

int ChannelManager::createChannel(ChannelInfo* channelInfo)
{
    channelInfo->channelNo = createChannelNo++;
    
    Channel* channel = new Channel();
    
    if(channel->initialize(channelInfo) == false)
    {
        ErrorLog("channel init error");
        return INTERNAL_ERROR;
    }
    
    channelList.push_back(channel);
    
    return channel->getChannelNo();
}


int ChannelManager::deleteChannel(int16_t channelNo)
{
    Channel* channel = NULL;
    for(itr = channelList.begin(); itr != channelList.end(); itr++)
    {
        channel = *itr;
        if(channel->getChannelNo() == channelNo)
        {
            channelList.erase(itr);
            return SUCCESS;
        }
    }
    
    return NOT_FOUND_CHANNEL;
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
            return channel;
        }
    }

	return NULL;
}


int ChannelManager::firstEnterUser(User* user)
{
    for(int i = 0; i < commonChannelCount; i++)
    {
        Channel* channel = getChannelByChannelNo(i);
        int failReason = channel->addUser(user);
        if (failReason != SUCCESS)
        {
            if(failReason == FULL_USER_IN_CHANNEL)
            {
                continue;
            }
            else
            {
                ErrorLog("FirstEnterUser");
                return INTERNAL_ERROR;
            }
        }
        return SUCCESS;
    }
    
    return FULL_USER_IN_CHANNEL;
}


ChannelManager::~ChannelManager()
{

}


