#define _CRT_SECURE_NO_WARNINGS

#include "Room.h"
#include "Log.h"
#include "User.h"
#include "GameDefine.h"
#include "GameServer.h"



Room::Room()
{
    
}

bool Room::initialize(RoomInfo* _roomInfo, int _userCount, User** _userList)
{
    roomInfo.roomNo = _roomInfo->roomNo;
    roomInfo.roomNameLen = _roomInfo->roomNameLen;
    memcpy(roomInfo.roomName, _roomInfo->roomName, roomInfo.roomNameLen);
    
    userCount = _userCount;
    userList = new User*[userCount];
    
    for(int i = 0; i < userCount; i++)
    {
        userList[i] = _userList[i];
    }
    
    roomState = ROOM_STATE_STAND_BY;
    
    return true;
}

bool Room::isPossibleStart()
{
    for(int i = 0; i <userCount; i++)
    {
        if(userList[i]->getConnectInfo() == NULL)
            return false;
        else
            userList[i]->setUserState(USER_STATE_GAME);
    }
    
    roomState = ROOM_STATE_START;
    
    return true;
}

void Room::finishGame()
{
    for(int i = 0; i <userCount; i++)
    {
        userList[i]->setRoomNo(INVALID_ROOM_NO);
        userList[i]->setRoom(NULL);
    }
}



//
//void Room::slotReset()
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        roomSlotList[i].isOpen = 1;
//        roomSlotList[i].isExistUser = 0;
//        roomSlotList[i].isReady = 0;
//        roomSlotList[i].tribe = 0;
//        roomSlotList[i].user = NULL;
//    }
//}

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
//
//int Room::joinUserInRoom(User* user)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 0)
//        {
//            roomSlotList[i].isExistUser = 1;
//            roomSlotList[i].user = user;
//            userCount++;
//            return SUCCESS;
//        }
//    }
//    
//    return -1;
//}
//
//bool Room::changeTribe(User* user, int tribe)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
//        {
//            if (roomSlotList[i].tribe == tribe)
//                return false;
//            
//            roomSlotList[i].tribe = tribe;
//            return true;
//        }
//    }
//    
//    return false;
//}
//
//bool Room::readyUserInRoom(User* user)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
//        {
//            if(i == masterIndex)
//            {
//                for (int j = 0; j < MAX_ROOM_SLOT_COUNT; j++)
//                {
//                    if (roomSlotList[j].isOpen == 1 && roomSlotList[j].isExistUser == 1 && roomSlotList[j].user != user)
//                    {
//                        if(roomSlotList[j].isReady != 1)
//                        {
//                            return false;
//                        }
//                    }
//                }
//                
//                roomSlotList[i].isReady = 1;
//                return true;
//                
//            }
//            else
//            {
//                if (roomSlotList[i].isReady == 1)
//                    return false;
//                
//                roomSlotList[i].isReady = 1;
//                return true;
//            }
//        }
//    }
//    
//    return false;
//}
//
//bool Room::unReadyUserInRoom(User* user)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
//        {
//            if (roomSlotList[i].isReady == 0)
//                return false;
//            
//            roomSlotList[i].isReady = 0;
//            return true;
//        }
//    }
//    
//    return false;
//}
//
//bool Room::leaveUserInRoom(User* user)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].user == user)
//        {
//            if (masterIndex == i)
//            {
//                giveMasterOtherUser();
//            }
//            
//            roomSlotList[i].isReady = 0;
//            roomSlotList[i].isExistUser = 0;
//            roomSlotList[i].user = NULL;
//            userCount--;
//            
//            for (int j = 0; j < MAX_ROOM_SLOT_COUNT; j++)
//            {
//                roomSlotList[j].isReady = 0;
//            }
//            
//            if (userCount == 0)
//            {
//                ErrorLog("??");//DestroyRoom();
//                return false;
//            }
//            
//            return true;
//        }
//    }
//    
//    return false;
//}
//
//
//void Room::giveMasterOtherUser()
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && masterIndex != i)
//        {
//            masterIndex = i;
//            return;
//        }
//    }
//    
//    masterIndex = -1;
//}
//
//
//void Room::getRoomSlot(RoomSlotInfo* roomSlot, int index)
//{
//    roomSlot->isOpen = roomSlotList[index].isOpen;
//    if(roomSlotList[index].isOpen == 1)
//    {
//        roomSlot->isExistUser = roomSlotList[index].isExistUser;
//        if(roomSlotList[index].isExistUser == 1)
//        {
//            roomSlot->isReady = roomSlotList[index].isReady;
//            roomSlot->tribe = roomSlotList[index].tribe;
//            roomSlot->user = roomSlot[index].user;
//        }
//    }
//}

//int Room::startGame()
//{
//    /*
//    if (m_userCount < 1 || m_roomState == PLAY_GAME)
//    {
//        return INTERNAL_ERROR;
//    }
//    
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (i != m_masterIndex && m_roomSlotList[i].IsExistUser == 1 && m_roomSlotList[i].IsReady == 0)
//        {
//            return ALL_USER_IS_NOT_READY;
//        }
//    }
//    
//    Time_t currentTime = CustomGetCurrentTime() - 100;
//    
//    m_roomState = PLAY_GAME;
//    
//    //	RoomTimer* roomTimer = new RoomTimer(WHOLE_TIME, 1000, 31000, this);
//    
//    //	m_timerManager->AddTimer(roomTimer);
//    
//    m_game = new Game();
//    if (m_game->Initialize(m_mapType) == false)
//    {
//        ErrorLog("game Init error - mapType = %d", m_mapType);
//        return INTERNAL_ERROR;
//    }
//    
//    m_game->StartGame(m_roomSlotList);
//    */
//    return SUCCESS;
//}




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

//int8_t Room::getIndexByUserNo(int32_t userNo)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user->getUserNo() == userNo)
//        {
//            return i;
//        }
//    }
//    
//    return -1;
//}
//
//int8_t Room::getIndexByUser(User* user)
//{
//    for (int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
//    {
//        if (roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user == user)
//        {
//            return i;
//        }
//    }
//    
//    return -1;
//}

Room::~Room()
{
    
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

Room* RoomManager::createRoom(RoomInfo* roomInfo, int userCount, UserInfoWithSessionId* userInfoWithSessionId)
{
    User* userList[userCount];
    
    
    for(int i = 0; i < userCount; i++)
    {
        int returnValue = GameServer::getInstance()->userMgr->addUnconnectedUser(&userInfoWithSessionId[i].userInfo, &userInfoWithSessionId[i].sessionId);
        if(returnValue != SUCCESS)
        {
            ErrorLog("??");
            return NULL;
        }
        
        int64_t userNo = userInfoWithSessionId[i].userInfo.userNo;
        
        userList[i] = GameServer::getInstance()->userMgr->getUnconnectedUserByUserNo(userNo);
        
        if(userList[i] == NULL)
        {
            ErrorLog("??");
            return NULL;
        }
        
        userList[i]->setRoomNo(roomInfo->roomNo);
    }
    
    
    Room* room = new Room();
    
    if(room->initialize(roomInfo, userCount, userList)== false)
    {
        ErrorLog("??");
        return NULL;
    }
    
    roomList.push_back(room);
    
    for(int i = 0; i < userCount; i++)
    {
        userList[i]->setRoom(room);
    }
    
    return room;
}

int RoomManager::destroyRoom(int32_t roomNo)
{
    Room* room;

    for( itr = roomList.begin(); itr != roomList.end(); itr++)
    {
        room = *itr;
        if(room->getRoomNo() == roomNo)
        {
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
//
//int RoomManager::joinRoom(int32_t roomNo, User* user)
//{
//    Room* room = getRoomByRoomNo(roomNo);
//    if(room == NULL)
//    {
//        ErrorLog("??");
//        return -1;
//    }
//    
//    
//    if (room->joinUserInRoom(user) != SUCCESS)
//    {
//        ErrorLog("??");
//        return -1;
//    }
//    
//    return SUCCESS;
//}
//
////int RoomManager::RandomJoinRoom(User* user)
////{
////    for (int i = 0; i < MAX_ROOM_COUNT; i++)
////    {
////        if (m_roomList[i].m_userCount > 0)
////        {
////            if (JoinRoom(i, user) == SUCCESS)
////            {
////                return SUCCESS;
////            }
////        }
////    }
////    return ROOM_SLOT_IS_FULL;
////}
//
//int RoomManager::readyRoom(User* user)
//{
//    int32_t roomNo = user->getRoomNo();
//    
//    Room* room = getRoomByRoomNo(roomNo);
//    if(room == NULL)
//    {
//        ErrorLog("??");
//        return -1;
//    }
//    
//    if (room->readyUserInRoom(user) == false)
//    {
//        return -1; chatting
//    }
//    
//    return SUCCESS;
//}
//
//
//int RoomManager::unReadyRoom(User* user)
//{
//    int32_t roomNo = user->getRoomNo();
//    
//    Room* room = getRoomByRoomNo(roomNo);
//    if(room == NULL)
//    {
//        ErrorLog("??");
//        return -1;
//    }
//    
//    if (room->unReadyUserInRoom(user) == false)
//    {
//        return -1;
//    }
//    
//    return SUCCESS;
//}
//
//
//
//int RoomManager::leaveRoom(User* user)
//{
//    int32_t roomNo = user->getRoomNo();
//    
//    Room* room = getRoomByRoomNo(roomNo);
//    if(room == NULL)
//    {
//        ErrorLog("??");
//        return -1;
//    }
//    
//    if(room->userCount == 1) //delete room
//    {
//        if(destroyRoom(roomNo) < 0)
//        {
//            ErrorLog("??");
//            return -1;
//        }
//        
//    }
//    else
//    {
//        if(room->leaveUserInRoom(user) == false)
//        {
//            ErrorLog("??");
//            return -1;
//        }
//        
//        return -1;
//    }
//    
//    return SUCCESS;
//}


Room* RoomManager::getRoomByRoomNo(int32_t roomNo)
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










//
//Channel::Channel()
//{
//
//}
//
//bool Channel::initialize(ChannelInfo* channelInfo)
//{
//    channelNo = channelInfo->channelNo;
//    channelNameLen = channelInfo->channelNameLen;
//    memset(channelName, 0, BasicPacket::MAX_CHANNEL_NAME_LEN);
//    memcpy(channelName, channelInfo->channelName, channelNameLen);
//    return true;
//}
//
//int Channel::addUser(User* user)
//{
//	userList.push_back(user);
//
//	return SUCCESS;
//}
//
//int Channel::removeUser(User* user)
//{
//	userList.remove(user);
//
//	return SUCCESS;
//}
//
//Channel::~Channel()
//{
//
//}
//
////ChannelManager
//
//ChannelManager::ChannelManager()
//{
//
//}
//
//bool ChannelManager::initialize(int channelCount, ChannelInfo* channelInfo)
//{
//    channelList.clear();
//    for (int i = 0; i < channelCount; i++)
//    {
//        Channel* channel = new Channel();
//        if(channel->initialize(&channelInfo[i]) == false)
//        {
//            ErrorLog("channel lnit error");
//            return false;
//        }
//        
//        channelList.push_back(channel);
//    }
//    
//    return true;
//}
//
//int16_t ChannelManager::getRandomChannelNo()
//{
//	return channelList.at(0)->getChannelNo();
//}
//
//Channel* ChannelManager::getChannelByChannelNo(int16_t channelNo)
//{
//    Channel* channel = NULL;
//    
//    for (itr = channelList.begin(); itr != channelList.end(); itr++)
//    {
//        
//        channel = *itr;
//        if(channel->getChannelNo() == channelNo)
//        {
//            DebugLog("channel %d", channelNo);
//            return channel;
//        }
//    }
//
//	return NULL;
//}
//
//int16_t ChannelManager::firstEnterUser(User* user)
//{
//	int channelNo = getRandomChannelNo();
//    
//    Channel* channel = getChannelByChannelNo(channelNo);
//	int failReason = channel->addUser(user);
//	if (failReason != SUCCESS)
//	{
//		ErrorLog("FirstEnterUser");
//		return INVALID_CHANNEL_NO;
//	}
//
//	return channelNo;
//}
//
//ChannelManager::~ChannelManager()
//{
//
//}
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