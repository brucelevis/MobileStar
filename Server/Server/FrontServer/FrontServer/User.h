
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include "BasicPacket.h"

enum USER_STATE
{
    USER_STATE_NOT_LOGIN,
    USER_STATE_LOGIN,
    USER_STATE_MOVING_LOBBY,
};

class ConnectInfo;

class User
{
public:

	User();

    bool initialize(SessionId_t* sid, ConnectInfo* _connectInfo);

	ConnectInfo* getConnectInfo() { return connectInfo; }
	void setConnectInfo(ConnectInfo* _connectInfo) { connectInfo = _connectInfo; }

	int64_t getUserNo() { return userNo; }
	void setUserNo(int64_t _userNo) { userNo = _userNo; }

    int8_t getUserState() { return userState; }
    void setUserState(int8_t _userState) { userState = _userState; }
    
    SessionId_t* getSid() { return &sid; }
    void setSid(SessionId_t _sid) { sid = _sid; }

    int getLobbyNo() { return lobbyNo; }
    void setLobbyNo(int _lobbyNo) { lobbyNo = _lobbyNo; }

	~User();

public:

	friend class UserManager;

private:
    SessionId_t sid;
	ConnectInfo* connectInfo;
    int64_t userNo;
    
    int8_t userState;
    int lobbyNo;
};


#include <boost/unordered_map.hpp>
#include <deque>

class UserManager
{
public:
	UserManager();

	bool initialize();
    int addUnconnectedUser(SessionId_t* sessionId, ConnectInfo* _connectInfo);
    
    
    int addConnectedUser(SessionId_t* sessionId, int64_t userNo);
    User* getUnConnectedUser(SessionId_t* sessionId);
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
