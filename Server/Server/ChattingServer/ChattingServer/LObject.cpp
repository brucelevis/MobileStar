#include "LObject.h"
#include "Log.h"
#include "User.h"
#include "ChattingDefine.h"
#include "ChattingServer.h"
#include "Network.h"
#include "ClientLobbyPacket.h"
#include "GameLobbyPacket.h"
#include "ClientChattingPacket.h"


LObject::LObject()
{
    
}

bool LObject::initialize(int32_t _loNo)
{
    loNo = _loNo;
    
    userCount = 0;
    
    userList.clear();
    
    return true;
}

int LObject::joinUser(User* user)
{
    userList.push_back(user);
    
    user->setLoctionObject(USER_LOCATION, this);
    
    return 0;
}

bool LObject::leaveUser(User* user)
{
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        if(*itr == user)
        {
            userList.erase(itr);
            user->setLoctionObject(USER_LOCATION_NO, NULL);
            return true;
        }
    }
    
    ErrorLog("??");
    return false;
}

void LObject::chattingNotify(User* user, char* chatting, uint8_t chattingLen)
{
    DebugLog("ClientReceiveHandler::chattingNotify");
    
    ClientChattingPacket::SendChattingNotifyPacket notifyPacket;
    notifyPacket.nickNameLen = user->getNickNameLen();
    memcpy(notifyPacket.nickName, user->getNickName(), notifyPacket.nickNameLen);
    
    notifyPacket.chattingLen = chattingLen;
    memcpy(notifyPacket.chatting, chatting, chattingLen);

    char* pNotifyBuffer = notifyBuffer;
    
    memcpy(pNotifyBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pNotifyBuffer += sizeof(notifyPacket.cmd);
    
    
    memcpy(pNotifyBuffer, &notifyPacket.nickNameLen, sizeof(notifyPacket.nickNameLen));
    pNotifyBuffer += sizeof(notifyPacket.nickNameLen);
    
    memcpy(pNotifyBuffer, notifyPacket.nickName, notifyPacket.nickNameLen);
    pNotifyBuffer += notifyPacket.nickNameLen;
    
    memcpy(pNotifyBuffer, &notifyPacket.chattingLen, sizeof(notifyPacket.chattingLen));
    pNotifyBuffer += sizeof(notifyPacket.chattingLen);
    
    memcpy(pNotifyBuffer, notifyPacket.chatting, notifyPacket.chattingLen);
    pNotifyBuffer += notifyPacket.chattingLen;
    
    
    User* notifyUser;
    
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        notifyUser = *itr;
        
        if(notifyUser->getConnectInfo() != NULL)
        {
            ChattingServer::getInstance()->network->sendPacket(notifyUser->getConnectInfo(), notifyBuffer, (int)(pNotifyBuffer - notifyBuffer));
        }
        else
        {
            ErrorLog("??");
        }
    }
        
    return;
}


LObject::~LObject()
{
    //ToDo.
}

//RoomManager

LObjectManager::LObjectManager()
{
    
}

bool LObjectManager::initialize()
{
    lObjectList.clear();
    
    return true;
}

LObject* LObjectManager::createLObject(int32_t loNo)
{
    LObject* lo = new LObject();
    if(lo->initialize(loNo)== false)
    {
        ErrorLog("??");
        return NULL;
    }
    
    
    lObjectList.push_back(lo);
    
    return lo;
}

int LObjectManager::destroyLObject(int32_t loNo)
{
//    Room* room;
//
//    for( itr = roomList.begin(); itr != roomList.end(); itr++)
//    {
//        room = *itr;
//        if(room->getRoomNo() == roomNo)
//        {
//            roomList.erase(itr);
//            delete room;
//            return SUCCESS;
//        }
//    }
    
    return -1;
}

bool LObjectManager::addUser(int32_t loNo, User* user)
{
    LObject* lo = getLObjectByLoNo(loNo);
    if(lo == NULL)
    {
        lo = createLObject(loNo);
        if(lo == NULL)
        {
            ErrorLog("??");
            return false;
        }
    }
    
    
    if (lo->joinUser(user) != SUCCESS)
    {
        ErrorLog("??");
        return false;
    }
    
    return true;
}


LObject* LObjectManager::getLObjectByLoNo(int32_t loNo)
{
    LObject* lo;
    
    for( itr = lObjectList.begin(); itr != lObjectList.end(); itr++)
    {
        lo = *itr;
        if(lo->getLObjectNo() == loNo)
        {
            return lo;
        }
    }
    
    return NULL;
}





