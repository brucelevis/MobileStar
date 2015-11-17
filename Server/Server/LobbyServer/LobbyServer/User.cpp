#include "User.h"
#include "Log.h"
#include "LobbyDefine.h"
#include "LobbyServer.h"
#include "Network.h"
#include "Clan.h"


User::User()
{
    
}

bool User::initialize(SessionId_t* _sid, int64_t userNo)
{
    memcpy(&sid, _sid, sizeof(SessionId_t));
    userInfo.userNo = userNo;
    roomNo = INVALID_ROOM_NO;
	channelNo = INVALID_CHANNEL_NO;
    userState = USER_STATE_PROCESS_STATE;
    userLocation = USER_LOCATION_NO;
    locationObject = NULL;
    
    requestGameUserNo = INVALID_USER_NO;
    return true;
}

void User::getUserInfo(UserInfo* _userInfo)
{
    _userInfo->userNo = userInfo.userNo;
    _userInfo->nickNameLen = userInfo.nickNameLen;
    memcpy(_userInfo->nickName, userInfo.nickName, userInfo.nickNameLen);
    _userInfo->commonWin = userInfo.commonWin;
    _userInfo->commonLose = userInfo.commonLose;
    _userInfo->commonDiss = userInfo.commonDiss;
    _userInfo->rankWin = userInfo.rankWin;
    _userInfo->rankLose = userInfo.rankLose;
    _userInfo->rankDiss = userInfo.rankDiss;
    _userInfo->grade = userInfo.grade;
    _userInfo->gradeReachedCount = userInfo.gradeReachedCount;
    _userInfo->point = userInfo.point;
    _userInfo->coin = userInfo.coin;
    _userInfo->clanNo = userInfo.clanNo;
    _userInfo->clanClass = userInfo.clanClass;
}

int User::getFriendList(NickNameInfoWithOnline* nickNameInfoWithOnlineList)
{
    int i;
    for(i = 0; i < friendList.size(); i++)
    {
        FriendUser* fu = friendList.at(i);
        nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen = fu->nickNameInfo.nickNameLen;
        memcpy(nickNameInfoWithOnlineList[i].nickNameInfo.nickName, fu->nickNameInfo.nickName, fu->nickNameInfo.nickNameLen);
        if(fu->user == NULL)
        {
            nickNameInfoWithOnlineList[i].online = 0;
        }
        else
        {
            nickNameInfoWithOnlineList[i].online = 1;
        }
    }
    
    return i;
}

bool User::setFriendUserOn(User* user)
{
    FriendUser* fu = NULL;
    
    std::vector<FriendUser*>::iterator itr;
    
    for(itr = friendList.begin(); itr != friendList.end(); itr++)
    {
        fu = *itr;
        
        if(memcmp(fu->nickNameInfo.nickName, user->getNickName(), user->getNickNameLen()) == 0)
        {
            if(fu->user != NULL)
            {
                ErrorLog("??");
            }
            
            fu->user = user;
            
            return true;
        }
    }
    
    return false;
}


bool User::setFriend(User* user)
{
    FriendUser* fu = NULL;
    
    std::vector<FriendUser*>::iterator itr;
    
    for(itr = friendList.begin(); itr != friendList.end(); itr++)
    {
        fu = *itr;
        
        if(memcmp(fu->nickNameInfo.nickName, user->getNickName(), user->getNickNameLen()) == 0)
        {
            ErrorLog("already friend");
            
            return false;
        }
    }
    
    fu = new FriendUser();
    fu->nickNameInfo.nickNameLen = user->getNickNameLen();
    memcpy(fu->nickNameInfo.nickName, user->getNickName(), user->getNickNameLen());
    fu->user = user;
    
    friendList.push_back(fu);
    
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
int UserManager::addUnconnectedUser(SessionId_t* sessionId, int64_t userNo)
{
    User* user = new User();
    if (user->initialize(sessionId, userNo) == false)
    {
        ErrorLog("user Initialize error");
        return INTERNAL_ERROR;
    }
    
    unconnectedUserList.push_back(user);
    
    return SUCCESS;
}


int UserManager::addConnectedUser(UserInfo* userInfo, int friendCount, NickNameInfo* friendList, ClanInfo* clanInfo)
{
    User* user = NULL;
    
    for(unconnectedUserListItr = unconnectedUserList.begin(); unconnectedUserListItr != unconnectedUserList.end(); unconnectedUserListItr++)
    {
        user = *unconnectedUserListItr;
        
        if(user->getUserNo() == userInfo->userNo)
        {
            unconnectedUserList.erase(unconnectedUserListItr);

            if (userMap.insert(std::map<std::string, User*>::value_type(std::string(userInfo->nickName, userInfo->nickNameLen), user)).second == false)
            {
                ErrorLog("user insert fail ");
                delete user;
                return ALREADY_EXIST_USER;
            }
            
            user->setUserInfo(userInfo);
            
            for(int i = 0; i < friendCount; i++)
            {
                
                User* otherUser = getUserByNickNameInfo(&friendList[i]);
                
                if(otherUser != NULL)
                {
                    otherUser->setFriendUserOn(user);
                }
                
                user->setFriend(otherUser);
            }
            
            if(userInfo->clanNo != INVALID_CLAN_NO)
            {
                ClanNameInfo clanNameInfo;
                clanNameInfo.clanNameLen = clanInfo->clanNameLen;
                memcpy(clanNameInfo.clanName, clanInfo->clanName, clanInfo->clanNameLen);
                
                Clan* clan = LobbyServer::getInstance()->clanMgr->getClanByClanNameInfo(&clanNameInfo);
                
                if(clan == NULL)
                {
                    if(LobbyServer::getInstance()->clanMgr->addClan(clanInfo) != SUCCESS)
                    {
                        ErrorLog("??");
                        return INTERNAL_ERROR;
                    }
                    
                    clan = LobbyServer::getInstance()->clanMgr->getClanByClanNameInfo(&clanNameInfo);
                    
                    if(clan == NULL)
                    {
                        ErrorLog("???");
                        return INTERNAL_ERROR;
                    }
                }
                
                clan->addUser(user);
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

User* UserManager::getUnconnectedUserBySessionId(SessionId_t *sessionId)
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


User* UserManager::getUserByNickNameInfo(NickNameInfo* nickNameInfo)
{
    std::map< std::string, User* >::const_iterator iter = userMap.find(std::string(nickNameInfo->nickName, nickNameInfo->nickNameLen));
    if (iter == userMap.end())
    {
        ErrorLog("not exist User");
        return NULL;
    }
    
	return iter->second;
}

User* UserManager::getUserByUserNo(int64_t userNo)
{
    User* user;
    
    std::map< std::string, User* >::iterator itr;
    
    for(itr = userMap.begin(); itr != userMap.end(); itr++)
    {
        user = itr->second;
        if(user->getUserNo() == userNo)
        {
            return user;
        }
    }
    
    return NULL;
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
    if (userMap.erase(std::string(user->userInfo.nickName, user->userInfo.nickNameLen)) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

	user->connectInfo = NULL;

	delete (user);

	return true;
}

bool UserManager::removeUserByNickNameInfo(NickNameInfo* nickNameInfo)
{
	User* user = getUserByNickNameInfo(nickNameInfo);

	if (user == 0)
	{
		ErrorLog("user not exist");
		return false;
	}

    if (userMap.erase(std::string(nickNameInfo->nickName, nickNameInfo->nickNameLen)) != 1)
	{
		ErrorLog("erase fail");
		return false;
	}

    user->connectInfo->userData = NULL;
    
	delete (user);

	return true;
}