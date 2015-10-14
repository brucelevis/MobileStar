#include "User.h"
#include "Log.h"
#include "LobbyDefine.h"
#include "Network.h"


User::User()
{
	
}

bool User::initialize(SessionId_t* _sid, NickNameInfo* _nickNameInfo)
{
    memcpy(&sid, _sid, sizeof(SessionId_t));
    memset(nickNameInfo.nickName, 0, MAX_NICK_NAME_LEN + 1);
    
    nickNameInfo.nickNameLen = _nickNameInfo->nickNameLen;
    
    memcpy(nickNameInfo.nickName, _nickNameInfo->nickName, nickNameInfo.nickNameLen);
    
    
    userLocation = USER_LOCATION_NO;
    locationObject = NULL;
    
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
	return true;
}
int UserManager::addUnconnectedUser(NickNameInfo* nickNameInfo, SessionId_t* sessionId)
{
    User* user = new User();
    if (user->initialize(sessionId, nickNameInfo) == false)
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

            std::string str(user->nickNameInfo.nickName, user->nickNameInfo.nickNameLen);
            
            if (userMap.insert(std::map<std::string, User*>::value_type(str, user)).second == false)
            {
                ErrorLog("user insert fail ");
                delete user;
                return ALREADY_EXIST_USER;
            }
            
            return SUCCESS;
        }
    }
    
    return INTERNAL_ERROR;
}


User* UserManager::getUnconnectedUserByNickNameInfo(NickNameInfo *nickNameInfo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(user->getNickNameLen() == nickNameInfo->nickNameLen && strncmp(user->getNickName(), nickNameInfo->nickName, user->getNickNameLen()) == 0)
        {
            return user;
        }
    }
    
    return NULL;
}


User* UserManager::getUserByNickNameInfo(NickNameInfo *nickNameInfo)
{
    std::map< std::string, User* >::const_iterator iter = userMap.find(std::string(nickNameInfo->nickName, nickNameInfo->nickNameLen));

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

bool UserManager::removeUnconnectedUserByNickNameInfo(NickNameInfo *nickNameInfo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(user->getNickNameLen() == nickNameInfo->nickNameLen && strncmp(user->getNickName(), nickNameInfo->nickName, user->getNickNameLen()) == 0)
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
    if (userMap.erase(std::string(user->nickNameInfo.nickName, user->nickNameInfo.nickNameLen)) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

	user->connectInfo = NULL;

	delete (user);

	return true;
}

bool UserManager::removeUserByNickNameInfo(NickNameInfo *nickNameInfo)
{
	User* user = getUserByNickNameInfo(nickNameInfo);

	if (user == 0)
	{
		ErrorLog("user not exist");
		return false;
	}

    if (userMap.erase(std::string(user->nickNameInfo.nickName, user->nickNameInfo.nickNameLen)) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

    user->connectInfo->userData = NULL;
    
	delete (user);

	return true;
}