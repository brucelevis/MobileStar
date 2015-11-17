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
    
    notifyPacket.nickNameInfo.nickNameLen = user->getNickNameLen();
    memcpy(notifyPacket.nickNameInfo.nickName, user->getNickName(), user->getNickNameLen());
    
    
    char* pNotifyBuffer = notifyBuffer;
    
    memcpy(pNotifyBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pNotifyBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pNotifyBuffer, &notifyPacket.nickNameInfo.nickNameLen, sizeof(notifyPacket.nickNameInfo.nickNameLen));
    pNotifyBuffer += sizeof(notifyPacket.nickNameInfo.nickNameLen);
    
    memcpy(pNotifyBuffer, &notifyPacket.nickNameInfo.nickName, notifyPacket.nickNameInfo.nickNameLen);
    pNotifyBuffer += notifyPacket.nickNameInfo.nickNameLen;
    
    
    
    
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
        packet.nickNameInfoList[i].nickNameLen = otherUser->getNickNameLen();
        memcpy(packet.nickNameInfoList[i].nickName, otherUser->getNickName(), otherUser->getNickNameLen());
        
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
        memcpy(pSendBuffer, &packet.nickNameInfoList[i].nickNameLen, sizeof(packet.nickNameInfoList[i].nickNameLen));
        pSendBuffer += sizeof(packet.nickNameInfoList[i].nickNameLen);
        
        memcpy(pSendBuffer, packet.nickNameInfoList[i].nickName, packet.nickNameInfoList[i].nickNameLen);
        pSendBuffer += packet.nickNameInfoList[i].nickNameLen;
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
    
    notifyPacket.nickNameInfo.nickNameLen = user->getNickNameLen();
    memcpy(notifyPacket.nickNameInfo.nickName, user->getNickName(), user->getNickNameLen());
    
    char* pSendBuffer = sendBuffer;

    memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pSendBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pSendBuffer, &notifyPacket.nickNameInfo.nickNameLen, sizeof(notifyPacket.nickNameInfo.nickNameLen));
    pSendBuffer += sizeof(notifyPacket.nickNameInfo.nickNameLen);
    
    memcpy(pSendBuffer, notifyPacket.nickNameInfo.nickName, notifyPacket.nickNameInfo.nickNameLen);
    pSendBuffer += notifyPacket.nickNameInfo.nickNameLen;
    
    std::list<User*>::iterator itr;
    
    User* otherUser = NULL;
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        otherUser = *itr;
        
        LobbyServer::getInstance()->network->sendPacket(otherUser->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
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


