
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include "BasicPacket.h"
#include <vector>
#include <string>

class ConnectInfo;


enum USER_LOCATION
{
    USER_LOCATION_NO = 0,
    USER_LOCATION,
};

class User
{
public:

	User();

    bool initialize(SessionId_t* sid, NickNameInfo* nickNameInfo);

	ConnectInfo* getConnectInfo() { return connectInfo; }
	void setConnectInfo(ConnectInfo* _connectInfo) { connectInfo = _connectInfo; }

	const char* getNickName() { return nickNameInfo.nickName; }
	int8_t getNickNameLen() { return nickNameInfo.nickNameLen; }
	void setNickName(char* _nickName, int8_t _nickNameLen)
	{
		nickNameInfo.nickNameLen = _nickNameLen;
		memcpy(nickNameInfo.nickName, _nickName, nickNameInfo.nickNameLen);
	}

    SessionId_t* getSid() { return &sid; }
    void setSid(SessionId_t _sid) { sid = _sid; }
    
    int8_t getUserLocation() { return userLocation; }
    void* getLocationObject() { return locationObject; }
    void setLoctionObject(int8_t _userLocation, void* _locationObject) { userLocation = _userLocation; locationObject = _locationObject; }

	~User();

public:

	friend class UserManager;

private:
    SessionId_t sid;
	ConnectInfo* connectInfo;
    
    NickNameInfo nickNameInfo;
    
    int8_t userLocation;
    void* locationObject;
};


#include <deque>
#include <map>

class UserManager
{
public:
	UserManager();

	bool initialize();
    int addUnconnectedUser(NickNameInfo* userInfo, SessionId_t* sessionId);
    
    int addConnectedUser(SessionId_t* sessionId, ConnectInfo* connectInfo);
	
    User* getUnconnectedUserByNickNameInfo(NickNameInfo* nickNameInfo);
    User* getUserByNickNameInfo(NickNameInfo* nickNameInfo);
    bool removeUnconnectedUserByUser(User* _user);
    bool removeUnconnectedUserByNickNameInfo(NickNameInfo* nickNameInfo);
    
    bool removeUser(User* _user);
	bool removeUserByNickNameInfo(NickNameInfo* nickNameInfo);

	~UserManager();

private:
    std::map<std::string, User*> userMap;
    std::deque<User*>::iterator unconnectedUserListItr;
    std::deque<User*> unconnectedUserList;
};

#endif //__USER_H__
