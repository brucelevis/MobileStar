
// create by yong chan kim
// since 2014.7.29


#ifndef __ROOM_H__
#define __ROOM_H__

#include <stdint.h>
#include <list>
#include <vector>
#include "BasicPacket.h"

class User;
class UserInfo;
class GameServerInfo;

enum ROOM_STATE
{
    ROOM_STATE_STAND_BY = 1,
    ROOM_STATE_START,
};


struct RoomSlotInfo
{
    int8_t isOpen;
    int8_t isExistUser;
    int8_t isReady;
    int8_t tribe;
    User* user;
};

class Room
{
public:

    Room();
    
    bool initialize(RoomInfo* roomInfo, User* user, int mapType);
    void slotReset();
    
    
    int joinUserInRoom(User* user);
    bool changeTribe(User* user, int tribe);
    bool readyUserInRoom(User* user);
    bool unReadyUserInRoom(User* user);
    bool leaveUserInRoom(User* user);
    
    void giveMasterOtherUser();
    
    int startGame();
    int startGameNotify(GameServerInfo* gameServerInfo);
    bool isPossibleToStart();
    void finishGame();
    
    int16_t getRoomNo() { return roomInfo.roomNo; }
    int getRoomNameLen() { return roomInfo.roomNameLen; }
    const char* getRoomName() { return roomInfo.roomName; }
    int8_t getMapType() { return mapType; }
    int8_t getRoomState() { return roomState; }
    
    int8_t getMasterIndex() { return masterIndex; }
    void getRoomSlot(RoomSlotInfo* roomSlot, int index);
    int8_t getIndexByUserNo(int32_t userNo);
    int8_t getIndexByUser(User* user);
    int8_t getUserCount() { return userCount; }
    
    const RoomSlotInfo* getRoomSlotList() { return roomSlotList; }
    
    int getGameServerNo() { return gameServerNo; }

    ~Room();

private:
	
    friend class RoomManager;
    
    RoomInfo roomInfo;
    int8_t userCount;
    int8_t masterIndex;
    int8_t roomState;
    RoomSlotInfo roomSlotList[MAX_ROOM_SLOT_COUNT];
    int8_t mapType;
    
    int gameServerNo;
};


class RoomManager
{
public:
    RoomManager();
    
    bool initialize();
    Room* createRoom(RoomInfo* roomInfo, User* user, int mapType);
    int destroyRoom(int16_t roomNo);
    int joinRoom(int16_t roomNo, User* user);
    int randomJoinRoom(User* user);
    int readyRoom(User* user);
    int unReadyRoom(User* user);
    int leaveRoom(User* user);

    
    Room* getRoomByRoomNo(int16_t roomNo);
    
    Room* getRoomByIndex(int index) { return roomList.at(index); }
    int getRoomCount() { return (int)roomList.size(); }
    
    ~RoomManager();
    
private:
    std::vector<Room*>::iterator itr;
    std::vector<Room*> roomList;
    unsigned long createRoomNo;
};


#endif //__ROOM_H__
