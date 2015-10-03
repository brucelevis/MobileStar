#include "User.h"
#include "Log.h"
#include "GameDefine.h"
#include "Network.h"
User::User()
{
	memset(this, 0, sizeof(User));

}

bool User::initialize(SessionId_t* _sid, UserInfo* _userInfo)
{
    memcpy(&sid, _sid, sizeof(SessionId_t));
    memcpy(&userInfo, _userInfo, sizeof(UserInfo));
	roomNo = INVALID_ROOM_NO;
	channelNo = INVALID_CHANNEL_NO;
    userState = USER_STATE_STAND_BY;
	return true;
}

User::~User()
{

}


UserManager::UserManager()
{

}

bool UserManager::initialize()
{
	userMap.clear();
	createUserNo = 1;
	return true;
}
int UserManager::addUnconnectedUser(UserInfo* userInfo, SessionId_t* sessionId)
{
    User* user = new User();
    if (user->initialize(sessionId, userInfo) == false)
    {
        ErrorLog("user Initialize error");
        return INTERNAL_ERROR;
    }
    
    unconnectedUserList.push_back(user);
    
    return SUCCESS;
}


int UserManager::addConnectedUser(SessionId_t* sessionId, ConnectInfo* connectInfo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(memcmp(user->getSid(), sessionId, sizeof(SessionId_t)) == 0)
        {
            unconnectedUserList.erase(unconnectedUserListItr);
            
            user->setConnectInfo(connectInfo);
            connectInfo->userData = (void*)user;

            if (userMap.insert(boost::unordered_map<int64_t, User*>::value_type(user->userInfo.userNo, user)).second == false)
            {
                ErrorLog("user insert fail userno = %lld", user->userInfo.userNo);
                delete user;
                return ALREADY_EXIST_USER;
            }
            
            return SUCCESS;
        }
    }
    
    return -1;
}

User* UserManager::getUnconnectedUserByUserNo(int64_t userNo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(user->getUserNo() == userNo)
        {
            return user;
        }
    }
    
    return NULL;
}


User* UserManager::getUserByUserNo(int64_t userNo)
{
	boost::unordered_map< int64_t, User* >::const_iterator iter = userMap.find(userNo);

	if (iter == userMap.end())
	{
		ErrorLog("not exist User");
		return NULL;
	}

	return iter->second;
}

bool UserManager::removeUser(User* user)
{
	if (userMap.erase(user->userInfo.userNo) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

	user->connectInfo = NULL;

	delete (user);

	return true;
}

bool UserManager::removeUserByUserNo(int64_t userNo)
{
	User* user = getUserByUserNo(userNo);

	if (user == 0)
	{
		ErrorLog("user not exist");
		return false;
	}

	if (userMap.erase(userNo) != 1)
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