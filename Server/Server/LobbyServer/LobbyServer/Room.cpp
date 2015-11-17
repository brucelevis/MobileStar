#include "Room.h"
#include "Log.h"
#include "User.h"
#include "LobbyDefine.h"
#include "LobbyServer.h"
#include "Network.h"
#include "ClientLobbyPacket.h"
#include "GameLobbyPacket.h"
#include "ServerInfo.h"
#include "Channel.h"


Room::Room()
{
    
}

bool Room::initialize(RoomInfo* _roomInfo, User* _user, int _mapType)
{
    roomInfo.roomNo = _roomInfo->roomNo;
    roomInfo.roomNameLen = _roomInfo->roomNameLen;
    memcpy(roomInfo.roomName, _roomInfo->roomName, roomInfo.roomNameLen);
    
    mapType = _mapType;
    slotReset();
    
    roomSlotList[0].isOpen = 1;
    roomSlotList[0].isExistUser = 1;
    roomSlotList[0].isReady = 0;
    roomSlotList[0].tribe = 0;
    roomSlotList[0].user = _user;
    
    userCount = 1;
    masterIndex = 0;
    roomState = ROOM_STATE_STAND_BY;
    
    if(_user->getUserLocation() == USER_LOCATION_CHANNEL)
    {
        Channel* channel = (Channel*)(_user->getLocationObject());
        channel->removeUser(_user);
    }
    _user->setRoomNo(roomInfo.roomNo);
    
    _user->setLoctionObject(USER_LOCATION_ROOM, this);
    
    _user->setChannelNo(INVALID_CHANNEL_NO);
    
    gameServerNo = -1;
    return true;
}

void Room::slotReset()
{
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        roomSlotList[i].isOpen = 1;
        roomSlotList[i].isExistUser = 0;
        roomSlotList[i].isReady = 0;
        roomSlotList[i].tribe = 0;
        roomSlotList[i].user = NULL;
    }
}

//bool Room::createRoom(User* user)
//{
//    if (m_userCount > 0)
//    {
//        ErrorLog("already Exist");
//        return false;
//    }
//    
//    Reset();
//    
//    m_userCount++;
//    m_masterIndex = 0;
//    m_roomSlotList[0].IsOpen = 1;
//    m_roomSlotList[0].IsExistUser = 1;
//    m_roomSlotList[0].user = user;
//    return true;
//}

//bool Room::DestroyRoom()
//{
//    DebugLog("DestroyRoom");
//    Reset();
//    return true;
//}

int Room::joinUserInRoom(User* user)
{
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 0)
        {
            roomSlotList[i].isExistUser = 1;
            roomSlotList[i].user = user;
            userCount++;
            user->setRoomNo(roomInfo.roomNo);
            if(user->getUserLocation() == USER_LOCATION_CHANNEL)
            {
                ErrorLog("??");
                Channel* channel = (Channel*)(user->getLocationObject());
                channel->removeUser(user);
            }
            
            user->setChannelNo(INVALID_CHANNEL_NO);
       
            user->setLoctionObject(USER_LOCATION_ROOM, (void*)this);
            return SUCCESS;
        }
    }
    
    return -1;
}

bool Room::changeTribe(User* user, int tribe)
{
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
        {
            if (roomSlotList[i].tribe == tribe)
                return false;
            
            roomSlotList[i].tribe = tribe;
            return true;
        }
    }
    
    return false;
}

bool Room::readyUserInRoom(User* user)
{
    if(roomState == ROOM_STATE_START)
    {
        return false;
    }
    
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
        {
            if(i == masterIndex)
            {
                for (int j = 0; j < MAX_ROOM_SLOT_COUNT; j++)
                {
                    if (roomSlotList[j].isOpen == 1 && roomSlotList[j].isExistUser == 1 && roomSlotList[j].user != user)
                    {
                        if(roomSlotList[j].isReady != 1)
                        {
                            return false;
                        }
                    }
                }
                
                roomSlotList[i].isReady = 1;
                roomState = ROOM_STATE_START;
                return true;
                
            }
            else
            {
                if (roomSlotList[i].isReady == 1)
                    return false;
                
                roomSlotList[i].isReady = 1;
                return true;
            }
        }
    }
    
    return false;
}

bool Room::unReadyUserInRoom(User* user)
{
    if(roomState == ROOM_STATE_START)
    {
        return false;
    }
    
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
        {
            if (roomSlotList[i].isReady == 0)
                return false;
            
            roomSlotList[i].isReady = 0;
            return true;
        }
    }
    
    return false;
}

bool Room::leaveUserInRoom(User* user)
{
    if(roomState == ROOM_STATE_START)
    {
        return false;
    }
    
    
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].user == user)
        {
            if (masterIndex == i)
            {
                giveMasterOtherUser();
            }
            
            roomSlotList[i].isReady = 0;
            roomSlotList[i].isExistUser = 0;
            roomSlotList[i].user = NULL;
            userCount--;
            
            for (int j = 0; j < MAX_ROOM_SLOT_COUNT; j++)
            {
                roomSlotList[j].isReady = 0;
            }
            
            if (userCount == 0)
            {
                ErrorLog("??");//DestroyRoom();
                return false;
            }
            
            user->setRoomNo(INVALID_ROOM_NO);
            
            user->setUserState(USER_STATE_PROCESS_STATE);
            user->setLoctionObject(USER_LOCATION_NO, NULL);
            
            
            
            /////////////send notify
            
            ClientLobbyPacket::LeaveRoomNotifyPacket notifyPacket;
            notifyPacket.leaveIndex = i;
            
            notifyPacket.roomSlot.isOpen = roomSlotList[i].isOpen;
            if(roomSlotList[i].isOpen == 1)
            {
                notifyPacket.roomSlot.isExistUser = roomSlotList[i].isExistUser;
                if(roomSlotList[i].isExistUser == 1)
                {
                    ErrorLog("??");
                    
                    notifyPacket.roomSlot.isReady = roomSlotList[i].isReady;
                    notifyPacket.roomSlot.tribe = roomSlotList[i].tribe;
                    notifyPacket.roomSlot.nickNameInfo.nickNameLen = roomSlotList[i].user->getNickNameLen();
                    memcpy(notifyPacket.roomSlot.nickNameInfo.nickName, roomSlotList[i].user->getNickName(), roomSlotList[i].user->getNickNameLen());
                }
            }
            
            
            char notifyBuffer[1000];
            
            char* pSendBuffer = notifyBuffer;
            
            memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
            pSendBuffer += sizeof(notifyPacket.cmd);
            
            memcpy(pSendBuffer, &notifyPacket.leaveIndex, sizeof(notifyPacket.leaveIndex));
            pSendBuffer += sizeof(notifyPacket.leaveIndex);
            
            memcpy(pSendBuffer, &notifyPacket.roomSlot.isOpen, sizeof(notifyPacket.roomSlot.isOpen));
            pSendBuffer += sizeof(notifyPacket.roomSlot.isOpen);
            
            if(notifyPacket.roomSlot.isOpen == 1)
            {
                memcpy(pSendBuffer, &notifyPacket.roomSlot.isExistUser, sizeof(notifyPacket.roomSlot.isExistUser));
                pSendBuffer += sizeof(notifyPacket.roomSlot.isExistUser);
                
                if(notifyPacket.roomSlot.isExistUser == 1)
                {
                    memcpy(pSendBuffer, &notifyPacket.roomSlot.isReady, sizeof(notifyPacket.roomSlot.isReady));
                    pSendBuffer += sizeof(notifyPacket.roomSlot.isReady);
                    
                    memcpy(pSendBuffer, &notifyPacket.roomSlot.tribe, sizeof(notifyPacket.roomSlot.tribe));
                    pSendBuffer += sizeof(notifyPacket.roomSlot.tribe);
                    
                    memcpy(pSendBuffer, &notifyPacket.roomSlot.nickNameInfo.nickNameLen, sizeof(notifyPacket.roomSlot.nickNameInfo.nickNameLen));
                    pSendBuffer += sizeof(notifyPacket.roomSlot.nickNameInfo.nickNameLen);
                    
                    memcpy(pSendBuffer, notifyPacket.roomSlot.nickNameInfo.nickName, notifyPacket.roomSlot.nickNameInfo.nickNameLen);
                    pSendBuffer += notifyPacket.roomSlot.nickNameInfo.nickNameLen;
                }
            }
            
            for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
            {
                if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
                {
                    LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), notifyBuffer, (int)(pSendBuffer - notifyBuffer));
                }
            }

            return true;
        }
    }
    
    return false;
}


void Room::giveMasterOtherUser()
{
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && masterIndex != i)
        {
            masterIndex = i;
            return;
        }
    }
    
    masterIndex = -1;
}


void Room::getRoomSlot(RoomSlotInfo* roomSlot, int index)
{
    roomSlot->isOpen = roomSlotList[index].isOpen;
    if(roomSlotList[index].isOpen == 1)
    {
        roomSlot->isExistUser = roomSlotList[index].isExistUser;
        if(roomSlotList[index].isExistUser == 1)
        {
            roomSlot->isReady = roomSlotList[index].isReady;
            roomSlot->tribe = roomSlotList[index].tribe;
            roomSlot->user = roomSlot[index].user;
        }
    }
}

int Room::startGame()
{
    roomState = ROOM_STATE_START;
    
    GameLobbyPacket::EnterRoomReqPacket packet;
    packet.roomNo = roomInfo.roomNo;
    
    int userIndex = 0;
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
        {
            roomSlotList[i].user->setUserState(USER_STATE_MOVING_GAME);
            memcpy(&packet.userInfoWithSessionId[userIndex].sessionId, roomSlotList[i].user->getSid(), sizeof(SessionId_t));
            memcpy(&packet.userInfoWithSessionId[userIndex].userInfo, roomSlotList[i].user->getUserInfo(), sizeof(UserInfo));
            userIndex++;
        }
    }
    
    packet.userCount = userCount;
    
    char sendBuffer[5000];
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);
    
    memcpy(pSendBuffer, &packet.roomNo, sizeof(packet.roomNo));
    pSendBuffer += sizeof(packet.roomNo);
    
    memcpy(pSendBuffer, &packet.userCount, sizeof(packet.userCount));
    pSendBuffer += sizeof(packet.userCount);
    
    for(int i = 0; i < packet.userCount; i++)
    {
        memcpy(pSendBuffer, &packet.userInfoWithSessionId[i].sessionId, sizeof(SessionId_t));
        pSendBuffer += sizeof(SessionId_t);
        
        memcpy(pSendBuffer, &packet.userInfoWithSessionId[i].userInfo, sizeof(UserInfo));
        pSendBuffer += sizeof(UserInfo);
    }
    
    GameServerInfo* gameServerInfo = LobbyServer::getInstance()->serverInfoMgr->getFreeGameServerInfo();
    
    LobbyServer::getInstance()->network->sendPacket(gameServerInfo->connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
    
    gameServerInfo->roomCount++;
    
    gameServerNo = gameServerInfo->gameServerNo;
    
    return SUCCESS;
}


int Room::startGameNotify(GameServerInfo* gameServerInfo)
{
    ClientLobbyPacket::StartGameNotifyPacket sendPacket;
    
    sendPacket.userCount = userCount;
    
    int userCnt = 0;
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
        {
            sendPacket.gameUserInfo[userCnt].nickNameInfo.nickNameLen = roomSlotList[i].user->getNickNameLen();
            memcpy(sendPacket.gameUserInfo[userCnt].nickNameInfo.nickName, roomSlotList[i].user->getNickName(), roomSlotList[i].user->getNickNameLen());
            sendPacket.gameUserInfo[userCnt].tribe = roomSlotList[i].tribe;
            userCnt++;
        }
    }
    
    memcpy(sendPacket.ip, gameServerInfo->clientIp, MAX_IP_ADDRESS_LEN);
    sendPacket.port = gameServerInfo->clientPort;
    
    if(userCnt != userCount)
    {
        ErrorLog("userCnt = %d, userCount = %d", userCnt, userCount);
        return INTERNAL_ERROR;
    }
    
    
    char notifyBuffer[1000];
    char* pSendBuffer = notifyBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.userCount, sizeof(sendPacket.userCount));
    pSendBuffer += sizeof(sendPacket.userCount);
    
    for(int i = 0; i < sendPacket.userCount; i++)
    {
        memcpy(pSendBuffer, &sendPacket.gameUserInfo[i].nickNameInfo.nickNameLen, sizeof(sendPacket.gameUserInfo[i].nickNameInfo.nickNameLen));
        pSendBuffer += sizeof(sendPacket.gameUserInfo[i].nickNameInfo.nickNameLen);
        
        memcpy(pSendBuffer, sendPacket.gameUserInfo[i].nickNameInfo.nickName, sendPacket.gameUserInfo[i].nickNameInfo.nickNameLen);
        pSendBuffer += sendPacket.gameUserInfo[i].nickNameInfo.nickNameLen;
        
        memcpy(pSendBuffer, &sendPacket.gameUserInfo[i].tribe, sizeof(sendPacket.gameUserInfo[i].tribe));
        pSendBuffer += sizeof(sendPacket.gameUserInfo[i].tribe);
    }
    
    memcpy(pSendBuffer, sendPacket.ip, MAX_IP_ADDRESS_LEN);
    pSendBuffer += MAX_IP_ADDRESS_LEN;
    
    memcpy(pSendBuffer, &sendPacket.port, sizeof(sendPacket.port));
    pSendBuffer += sizeof(sendPacket.port);
    
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
        {
            LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), notifyBuffer, (int)(pSendBuffer - notifyBuffer));
        }
    }
    
    return SUCCESS;
}



bool Room::isPossibleToStart()
{
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
        {
            if(roomSlotList[i].user->getUserState() == USER_STATE_GAME)
            {
                continue;
            }
            else if (roomSlotList[i].user->getUserState() == USER_STATE_MOVING_GAME)
            {
                return false;
            }
            else
            {
                ErrorLog("??");
                return false;
            }
        }
    }
    
    return true;
}


/*
 int Room::SolveProblem(int8_t index, int8_t selectType, int16_t* acquiredScore)
 {
	*acquiredScore = 0;
	if (index < 0 || index >= MAX_ROOM_SLOT_COUNT || selectType < DONUT1 || selectType > DONUT3)
	{
 ErrorLog("??");
 return INTERNAL_ERROR;
	}
 
	if (m_roomState == WAIT_ROOM)
	{
 return ALREADY_FINISH_GAME;
	}
 
	if (CustomGetCurrentTime() - m_roomSlotList[index].lastIncorrectSolveTime < 600)
	{
 ErrorLog("Incorrect Delay");
 return INCORRECT_SOLVE_DELAY;
	}
 
 
	if (m_problemSet->problemInfo[m_roomSlotList[index].currentIndex].i_type == selectType)
	{
 *acquiredScore = m_problemSet->problemScore[m_roomSlotList[index].currentIndex];
 m_roomSlotList[index].currentIndex++;
 m_roomSlotList[index].score += *acquiredScore;
 if (m_roomSlotList[index].currentIndex == PROBLEM_INDEX_COUNT) m_roomSlotList[index].currentIndex = 0;
 return SUCCESS;
	}
	else
	{
 m_roomSlotList[index].lastIncorrectSolveTime = CustomGetCurrentTime();
 return INCORRECT_PROBLEM_SOLVE;
	}
 }
 */
void Room::finishGame() // ToDo. 보상 받으면 줘야됨.
{
    User* user = NULL;
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
        {
            user = roomSlotList[i].user;
            
            if(LobbyServer::getInstance()->userMgr->addUnconnectedUser(user->getSid(), user->getUserNo()) != SUCCESS)
            {
                ErrorLog("???");
                return ;
            }
            
            if(LobbyServer::getInstance()->userMgr->removeUser(user) == false)
            {
                ErrorLog("??");
                return ;
            }
        }
    }
}

int8_t Room::getIndexByUserNo(int32_t userNo)
{
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user->getUserNo() == userNo)
        {
            return i;
        }
    }
    
    return -1;
}

int8_t Room::getIndexByUser(User* user)
{
    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
        {
            return i;
        }
    }
    
    return -1;
}

Room::~Room()
{
    //ToDo.
}

//RoomManager

RoomManager::RoomManager()
{
    
}

bool RoomManager::initialize()
{
    roomList.clear();
    createRoomNo = 1;
    return true;
}

Room* RoomManager::createRoom(RoomInfo* roomInfo, User* user, int mapType)
{
    roomInfo->roomNo = createRoomNo++;
    
    Room* room = new Room();
    if(room->initialize(roomInfo, user, mapType)== false)
    {
        ErrorLog("??");
        return NULL;
    }
    
    
    roomList.push_back(room);
    
    return room;
}

int RoomManager::destroyRoom(int16_t roomNo)
{
    Room* room;

    for( itr = roomList.begin(); itr != roomList.end(); itr++)
    {
        room = *itr;
        if(room->getRoomNo() == roomNo)
        {
            LobbyServer::getInstance()->serverInfoMgr->decreaseGameServerRoomCount(room->gameServerNo);
            roomList.erase(itr);
            delete room;
            return SUCCESS;
        }
    }
    
    return -1;
    
//
//    if (roomNo < 0 || roomNo > MAX_ROOM_COUNT - 1)
//    {
//        return INTERNAL_ERROR;
//    }
//    
//    if (m_roomList[roomNo].m_userCount > 0)
//    {
//        return INTERNAL_ERROR;
//    }
//    
//    m_roomList[roomNo].DestroyRoom();
//
}

int RoomManager::joinRoom(int16_t roomNo, User* user)
{
    Room* room = getRoomByRoomNo(roomNo);
    if(room == NULL)
    {
        ErrorLog("??");
        return -1;
    }
    
    
    if (room->joinUserInRoom(user) != SUCCESS)
    {
        ErrorLog("??");
        return -1;
    }
    
    user->setRoomNo(roomNo);
    
    return SUCCESS;
}


int RoomManager::readyRoom(User* user)
{
    int16_t roomNo = user->getRoomNo();
    
    Room* room = getRoomByRoomNo(roomNo);
    if(room == NULL)
    {
        ErrorLog("??");
        return -1;
    }
    
    if (room->readyUserInRoom(user) == false)
    {
        return -1;
    }
    
    return SUCCESS;
}


int RoomManager::unReadyRoom(User* user)
{
    int16_t roomNo = user->getRoomNo();
    
    Room* room = getRoomByRoomNo(roomNo);
    if(room == NULL)
    {
        ErrorLog("??");
        return -1;
    }
    
    if (room->unReadyUserInRoom(user) == false)
    {
        return -1;
    }
    
    return SUCCESS;
}



int RoomManager::leaveRoom(User* user)
{
    Room* room = (Room*)user->getLocationObject();
    if(room == NULL)
    {
        ErrorLog("??");
        return INTERNAL_ERROR;
    }
    
    if(room->leaveUserInRoom(user) == false)
    {
        ErrorLog("??");
        return INTERNAL_ERROR;
    }
    
    if(room->userCount == 0) //delete room
    {
        if(destroyRoom(room->getRoomNo()) < 0)
        {
            ErrorLog("??");
            return INTERNAL_ERROR;
        }
    }
    
    return SUCCESS;
}


Room* RoomManager::getRoomByRoomNo(int16_t roomNo)
{
    Room* room;
    
    for( itr = roomList.begin(); itr != roomList.end(); itr++)
    {
        room = *itr;
        if(room->getRoomNo() == roomNo)
        {
            return room;
        }
    }
    
    return NULL;
}





