#include "User.h"
#include "Log.h"
#include "ChattingDefine.h"

User::User()
{
	memset(this, 0, sizeof(User));

}
/*
bool User::Initialize(Session* session, SessionId_t sid, UserInfo* userInfo)
{
	m_session = session;
	m_tempSid = sid;
	memcpy(&m_userInfo, userInfo, sizeof(UserInfo));
	m_roomNo = INVALID_ROOM_NO;
	m_channelNo = INVALID_CHANNEL_NO;
	return true;
}

User::~User()
{

}
*/

UserManager::UserManager()
{

}
/*
bool UserManager::Initialize()
{
	m_userMap.clear();
	m_createUserNo = 1;
	return true;
}

int UserManager::AddUser(Session* session, SessionId_t sid, UserInfo* userInfo)
{
	User* user = new User();
	if (user->Initialize(session, sid, userInfo) == false)
	{
		ErrorLog("user Initialize error");
		return INTERNAL_ERROR;
	}
	if (m_userMap.insert(boost::unordered_map<int64_t, User*>::value_type(user->m_userInfo.userNo, user)).second == false)
	{
		ErrorLog("user insert fail userno = %lld", user->m_userInfo.userNo);
		delete user;
		return ALREADY_EXIST_USER;
	}

//	session->userKey = user->m_userInfo.userNo;

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
	if (m_userMap.erase(user->m_userInfo.userNo) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

	user->m_session = NULL;

	delete (user);

	return true;
}

bool UserManager::RemoveUserByUserNo(int64_t userNo)
{
	User* user = GetUserByUserNo(userNo);

	if (user == 0)
	{
		ErrorLog("user not exist");
		return false;
	}

	if (m_userMap.erase(userNo) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

	delete (user);

	return true;
}
/*

UserManager::UserManager()
{

}

bool UserManager::Initialize()
{
	m_userMap.clear();
	m_createUserNo = 1;
	return true;
}

int UserManager::AddUser(Session* session, const char* nickName, int nickNameLen)
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