#include "User.h"
#include "Log.h"
#include "LobbyDefine.h"
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
    userState = USER_STATE_PROCESS_STATE;
    userLocation = USER_LOCATION_NO;
    locationObject = NULL;
    
    requestGameUserNo = INVALID_USER_NO;
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
    
    return INTERNAL_ERROR;
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

bool UserManager::removeUnconnectedUserByUser(User* _user)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(user == _user)
        {
            unconnectedUserList.erase(unconnectedUserListItr);
            user->connectInfo->userData = NULL;
            
            delete (user);
            return true;
        }
    }
    
    return false;
}

bool UserManager::removeUnconnectedUserByUserNo(int64_t userNo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(user->getUserNo() == userNo)
        {
            unconnectedUserList.erase(unconnectedUserListItr);
            
            if(user->connectInfo != NULL)
            {
                user->connectInfo->userData = NULL;
            }
            delete (user);
            return true;
        }
    }
    
    return false;
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

    user->connectInfo->userData = NULL;
    
	delete (user);

	return true;
}