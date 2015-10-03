
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


struct RoomSlotInfo
{
    int8_t isOpen;
    int8_t isExistUser;
    int8_t isReady;
    int8_t tribe;
    User* user;
};

enum ROOM_STATE
{
    ROOM_STATE_STAND_BY = 1,
    ROOM_STATE_START,
    ROOM_STATE_FINISH,
};

class Room
{
public:

    Room();
    
    bool initialize(RoomInfo* roomInfo, int userCount, User** userList);
    
    bool isPossibleStart();
//    void slotReset();
//    
//    
//    int joinUserInRoom(User* user);
//    bool changeTribe(User* user, int tribe);
//    bool readyUserInRoom(User* user);
//    bool unReadyUserInRoom(User* user);
//    bool leaveUserInRoom(User* user);
//    
//    void giveMasterOtherUser();
//    
//    int startGame();
//    
    void finishGame();
    
    int32_t getRoomNo() { return roomInfo.roomNo; }
    int getRoomNameLen() { return roomInfo.roomNameLen; }
    const char* getRoomName() { return roomInfo.roomName; }
    int8_t getUserCount() { return userCount; }
    User** getUserList() { return userList; }
    
    int8_t getRoomState() { return roomState; }
    void setRoomState(int8_t _roomState) { roomState = _roomState; }
//    int8_t getMapType() { return mapType; }
//    
//    int8_t getMasterIndex() { return masterIndex; }
//    void getRoomSlot(RoomSlotInfo* roomSlot, int index);
//    int8_t getIndexByUserNo(int32_t userNo);
//    int8_t getIndexByUser(User* user);
//    
//    const RoomSlotInfo* getRoomSlotList() { return roomSlotList; }
    
//    void getRoomSlotList(BasicPacket::RoomSlot** roomSlotList);
    
    ~Room();

private:
	
    friend class RoomManager;
    
    RoomInfo roomInfo;
    int8_t roomState;
    int8_t userCount;
    User** userList;
};


class RoomManager
{
public:
    RoomManager();
    
    bool initialize();
    Room* createRoom(RoomInfo* roomInfo, int userCount, UserInfoWithSessionId* userInfoWithSessionId);
    int destroyRoom(int32_t roomNo);
//    int joinRoom(int32_t roomNo, User* user);
//    int randomJoinRoom(User* user);
//    int readyRoom(User* user);
//    int unReadyRoom(User* user);
//    int leaveRoom(User* user);

    
    Room* getRoomByRoomNo(int32_t roomNo);
    
    Room* getRoomByIndex(int index) { return roomList.at(index); }
    int getRoomCount() { return (int)roomList.size(); }
    
    ~RoomManager();
    
private:
    std::vector<Room*>::iterator itr;
    std::vector<Room*> roomList;
    unsigned long createRoomNo;
};


#endif //__ROOM_H__
