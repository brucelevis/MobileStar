
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include "BasicPacket.h"

enum USER_STATE
{
    USER_STATE_LOGIN,
    USER_STATE_MOVING_LOBBY,
};

class ConnectInfo;

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

    int8_t getUserState() { return userState; }
    void setUserState(int8_t _userState) { userState = _userState; }
    
    SessionId_t* getSid() { return &sid; }
    void setSid(SessionId_t _sid) { sid = _sid; }

	UserInfo* getUserInfo() { return &userInfo; }

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
	User* getUserByUserNo(int64_t userNo);
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
