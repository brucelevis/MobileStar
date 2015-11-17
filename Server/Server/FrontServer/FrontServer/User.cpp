#include "User.h"
#include "Log.h"
#include "FrontDefine.h"
#include "Network.h"

User::User()
{
	memset(this, 0, sizeof(User));

}

bool User::initialize(SessionId_t* _sid, ConnectInfo* _connectInfo)
{
    memcpy(&sid, _sid, sizeof(SessionId_t));
    connectInfo = _connectInfo;
    userNo = INVALID_USER_NO;
    userState = USER_STATE_NOT_LOGIN;
    lobbyNo = -1;
    
    connectInfo->userData = (void*)this;
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
int UserManager::addUnconnectedUser(SessionId_t* sessionId, ConnectInfo* _connectInfo)
{
    User* user = new User();
    if (user->initialize(sessionId, _connectInfo) == false)
    {
        ErrorLog("user Initialize error");
        return INTERNAL_ERROR;
    }
    
    unconnectedUserList.push_back(user);
    
    return SUCCESS;
}


int UserManager::addConnectedUser(SessionId_t* sessionId, int64_t userNo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(memcmp(user->getSid(), sessionId, sizeof(SessionId_t)) == 0)
        {
            unconnectedUserList.erase(unconnectedUserListItr);
            
            user->setUserNo(userNo);
            
            if (userMap.insert(boost::unordered_map<int64_t, User*>::value_type(user->userNo, user)).second == false)
            {
                ErrorLog("user insert fail userno = %lld", user->userNo);
                delete user;
                return ALREADY_EXIST_USER;
            }
            
            user->setUserState(USER_STATE_LOGIN);
            
            return SUCCESS;
        }
    }
    
    return -1;
}

User* UserManager::getUnConnectedUser(SessionId_t* sessionId)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(memcmp(user->getSid(), sessionId, sizeof(SessionId_t)) == 0)
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
	if (userMap.erase(user->userNo) != 1)
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


