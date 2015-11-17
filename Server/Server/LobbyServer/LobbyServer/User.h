
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include "BasicPacket.h"
#include <vector>

class ConnectInfo;
class User;

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

struct FriendUser
{
    NickNameInfo nickNameInfo;
    User* user;
};

class User
{
public:

	User();

    bool initialize(SessionId_t* _sid, int64_t userNo);

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
    void setUserInfo(UserInfo* _userInfo) { memcpy(&userInfo, _userInfo, sizeof(UserInfo)); }
    
    int8_t getUserLocation() { return userLocation; }
    void* getLocationObject() { return locationObject; }
    void setLoctionObject(int8_t _userLocation, void* _locationObject) { userLocation = _userLocation; locationObject = _locationObject; }

    void setRequestGameUserNo(int64_t _userNo) { requestGameUserNo = _userNo; }
    int64_t getRequestGameUserNo() { return requestGameUserNo; }
    
    bool setFriendUserOn(User* user);
    bool setFriend(User* user);
    
    void getUserInfo(UserInfo* userInfo);
    
    int getFriendList(NickNameInfoWithOnline* nickNameInfo);
    
    int32_t getClanNo() { return userInfo.clanNo; }
    
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
    
    std::vector<FriendUser*> friendList;
};


#include <boost/unordered_map.hpp>
#include <deque>

class UserManager
{
public:
	UserManager();

	bool initialize();
    int addUnconnectedUser(SessionId_t* sessionId, int64_t userNo);
    
    int addConnectedUser(UserInfo* userInfo, int friendCount, NickNameInfo* friendList, ClanInfo* clanInfo);
	
    User* getUnconnectedUserByUserNo(int64_t userNo);
    User* getUnconnectedUserBySessionId(SessionId_t* sessionId);

    User* getUserByNickNameInfo(NickNameInfo* nickNameInfo);
    User* getUserByUserNo(int64_t userNo);
    
    bool removeUnconnectedUserByUser(User* _user);
    bool removeUnconnectedUserByUserNo(int64_t userNo);
    
    bool removeUser(User* _user);
	bool removeUserByNickNameInfo(NickNameInfo* nickNameInfo);

	~UserManager();

private:
    std::map<std::string, User*> userMap;
//	boost::unordered_map< int64_t, User* > userMap;
    std::deque<User*>::iterator unconnectedUserListItr;
    std::deque<User*> unconnectedUserList;
	long createUserNo;
    
    
};

#endif //__USER_H__
