
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include "Channel.h"
#include "BasicPacket.h"

const int INVALID_USER_NO = -1;
const int INVALID_ROOM_NO = -1;
const int INVALID_CHANNEL_NO = -1;

struct UserInfo
{
	int64_t userNo;
	int8_t nickNameLen;
	char nickName[BasicPacket::MAX_NICK_NAME_LEN + 1];
	int32_t gold;
	int8_t memoLen;
	char memo[BasicPacket::MAX_MEMO_LEN + 1];
	int16_t commonWin;
	int16_t commonLose;
	int16_t commonDiss;
	int16_t rankWin;
	int16_t rankLose;
	int16_t rankDiss;
	int8_t grade;
	int8_t gradeReachedCount;
	int8_t point;
	int8_t icon;
};

struct NickNameInfo
{
	int8_t nickNameLen;
	char nickName[BasicPacket::MAX_NICK_NAME_LEN + 1];
};

struct RequestInfo
{
	int16_t requestType;
	NickNameInfo nickNameInfo;
};

class User
{
public:

	User();
/*
	bool Initialize(Session* session, SessionId_t sid, UserInfo* userInfo);

	Session* GetSession() { return m_session; }
	void SetSession(Session* session) { m_session = session; }

	int64_t GetUserNo() { return m_userInfo.userNo; }
	void SetUserNo(int64_t userNo) { m_userInfo.userNo = userNo; }

	const char* GetNickName() { return m_userInfo.nickName; }
	int8_t GetNickNameLen() { return m_userInfo.nickNameLen; }
	void SetNickName(char* nickName, int8_t nickNameLen)
	{
		m_userInfo.nickNameLen = nickNameLen;
		memcpy(m_userInfo.nickName, nickName, m_userInfo.nickNameLen);
	}

	int8_t GetChannelNo() { return m_channelNo; }
	void SetChannelNo(int16_t channelNo) { m_channelNo = channelNo; }

	int8_t GetRoomNo() { return m_roomNo; }
	void SetRoomNo(int16_t roomNo) { m_roomNo = roomNo; }

	SessionId_t GetTempSid() { return m_tempSid; }
	void SetTempSid(SessionId_t sid) { m_tempSid = sid; }

	UserInfo* GetUserInfo() { return &m_userInfo; }

	~User();

public:

	friend class Channel;
	friend class UserManager;

private:
//	SessionId_t m_tempSid;
//	Session* m_session;
	UserInfo m_userInfo;

	int16_t m_channelNo;
	int16_t m_roomNo;
*/
};


#include <boost/unordered_map.hpp>

class UserManager
{
public:
	UserManager();
/*
	bool Initialize();
	int AddUser(Session* session, SessionId_t sid, UserInfo* userInfo);
	User* GetUserByUserNo(int64_t userNo);
	bool RemoveUser(User* user);
	bool RemoveUserByUserNo(int64_t userNo);



	~UserManager();

private:
	boost::unordered_map< int64_t, User* > m_userMap;
	long m_createUserNo;*/
};

#endif //__USER_H__
