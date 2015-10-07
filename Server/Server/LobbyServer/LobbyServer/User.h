
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include "BasicPacket.h"
#include <vector>

class ConnectInfo;

enum USER_STATE
{
    USER_STATE_PROCESS_STATE = 0,
    USER_STATE_STAND_BY,
    USER_STATE_LOBBY,
    USER_STATE_WAIT_START_IN_LOBBY,
    USER_STATE_ROOM,
    USER_STATE_MOVING_GAME,
    USER_STATE_GAME,
};

enum USER_LOCATION
{
    USER_LOCATION_NO = 0,
    USER_LOCATION_CHANNEL,
    USER_LOCATION_ROOM,
};

class User
{
public:

	User();

    bool initialize(SessionId_t* sid, UserInfo* userInfo);

	ConnectInfo* getConnectInfo() { return connectInfo; }
	void setConnectInfo(ConnectInfo* _connectInfo) { connectInfo = _connectInfo; }

	int64_t getUserNo() { return userInfo.userNo; }
	void setUserNo(int64_t userNo) { userInfo.userNo = userNo; }

	const char* getNickName() { return userInfo.nickName; }
	int8_t getNickNameLen() { return userInfo.nickNameLen; }
	void setNickName(char* nickName, int8_t nickNameLen)
	{
		userInfo.nickNameLen = nickNameLen;
		memcpy(userInfo.nickName, nickName, userInfo.nickNameLen);
	}

	int16_t getChannelNo() { return channelNo; }
	void setChannelNo(int16_t _channelNo) { channelNo = _channelNo; }

    int16_t getRoomNo() { return roomNo; }
    void setRoomNo(int16_t _roomNo) { roomNo = _roomNo; }
    
    int16_t getUserState() { return userState; }
    void setUserState(int16_t _userState) { userState = _userState; }

    SessionId_t* getSid() { return &sid; }
    void setSid(SessionId_t _sid) { sid = _sid; }

	UserInfo* getUserInfo() { return &userInfo; }
    
    int8_t getUserLocation() { return userLocation; }
    void* getLocationObject() { return locationObject; }
    void setLoctionObject(int8_t _userLocation, void* _locationObject) { userLocation = _userLocation; locationObject = _locationObject; }

    void setRequestGameUserNo(int64_t _userNo) { requestGameUserNo = _userNo; }
    int64_t getRequestGameUserNo() { return requestGameUserNo; }
    
    void addResponseGameUserNo(int64_t _userNo) { responseGameUserNoVector.push_back(_userNo); }
    void removeResponseGameUserNo(int64_t _userNo);
    
	~User();

public:

	friend class UserManager;

private:
    SessionId_t sid;
	ConnectInfo* connectInfo;
	UserInfo userInfo;

	int16_t channelNo;
	int16_t roomNo;
    int8_t userState;

    int8_t userLocation;
    void* locationObject;
    
    int64_t requestGameUserNo;
    std::vector<int64_t> responseGameUserNoVector;
    std::vector<int64_t>::iterator rguvItr;
};


#include <boost/unordered_map.hpp>
#include <deque>

class UserManager
{
public:
	UserManager();

	bool initialize();
    int addUnconnectedUser(UserInfo* userInfo, SessionId_t* sessionId);
    
    int addConnectedUser(SessionId_t* sessionId, ConnectInfo* connectInfo);
	
    User* getUnconnectedUserByUserNo(int64_t userNo);
    User* getUserByUserNo(int64_t userNo);
    bool removeUnconnectedUserByUser(User* _user);
    bool removeUnconnectedUserByUserNo(int64_t userNo);
    
    bool removeUser(User* _user);
	bool removeUserByUserNo(int64_t userNo);

    
    


	~UserManager();

private:
	boost::unordered_map< int64_t, User* > userMap;
    std::deque<User*>::iterator unconnectedUserListItr;
    std::deque<User*> unconnectedUserList;
	long createUserNo;
};

#endif //__USER_H__
