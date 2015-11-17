#include "User.h"
#include "Log.h"
#include "CacheDefine.h"
#include "Network.h"

User::User()
{

}

bool User::initialize(std::string _userId, UserInfo* _userInfo)
{
    userId = _userId;
    userNo = _userInfo->userNo;
    nickNameLen = _userInfo->nickNameLen;
    DebugLog("%d", nickNameLen);
    memcpy(nickName, _userInfo->nickName, nickNameLen);
    commonWin = _userInfo->commonWin;
    commonLose = _userInfo->commonLose;
    commonDiss = _userInfo->commonDiss;
    rankWin = _userInfo->rankWin;
    rankLose = _userInfo->rankLose;
    rankDiss = _userInfo->rankDiss;
    grade = _userInfo->grade;
    gradeReachedCount = _userInfo->gradeReachedCount;
    point = _userInfo->point;
    coin = _userInfo->coin;
    clanNo = _userInfo->clanNo;
    clanClass = _userInfo->clanClass;
    
    friendList.clear();
    return true;
}

void User::getUserInfo(UserInfo* userInfo)
{
    userInfo->userNo = userNo;
    userInfo->nickNameLen = nickNameLen;
    memcpy(userInfo->nickName, nickName, nickNameLen);
    userInfo->commonWin = commonWin;
    userInfo->commonLose = commonLose;
    userInfo->commonDiss = commonDiss;
    userInfo->rankWin = rankWin;
    userInfo->rankLose = rankLose;
    userInfo->rankDiss = rankDiss;
    userInfo->grade = grade;
    userInfo->gradeReachedCount = gradeReachedCount;
    userInfo->point = point;
    userInfo->coin = coin;
    userInfo->clanNo = clanNo;
    userInfo->clanClass = clanClass;
    
}

int User::getFriendList(NickNameInfo* nickNameInfo)
{
    int i;
    for(i = 0; i < friendList.size(); i++)
    {
        User* user = friendList.at(i);
        nickNameInfo[i].nickNameLen = user->getNickNameLen();
        memcpy(nickNameInfo[i].nickName, user->getNickName(), nickNameInfo[i].nickNameLen);
    }
    
    return i;
}

int User::loginUser(UserInfo* userInfo)
{
    return SUCCESS;
}

int User::logoutUser()
{
    return SUCCESS;
}


bool User::addFriend(User* user)
{
    if(friendList.size() == MAX_FRIEND_COUNT)
    {
        DebugLog("full friend");
        return false;
    }
    
    std::vector<User*>::iterator itr;
    
    for(itr = friendList.begin(); itr != friendList.end(); itr++)
    {
        if(user == *itr)
        {
            DebugLog("already friend");
            return false;
        }
    }
    
    
    friendList.push_back(user);
    
    return true;
}


bool User::removeFriend(User* user)
{
    std::vector<User*>::iterator itr;
    
    for(itr = friendList.begin(); itr != friendList.end(); itr++)
    {
        if(user == *itr)
        {
            friendList.erase(itr);
            return true;
        }
    }
    
    ErrorLog("??");
    
    return false;
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
    userIdMap.clear();
    userNoMap.clear();
    
    return true;
}

int UserManager::addUser(std::string userId, UserInfo* userInfo)
{
    User* user = new User();
    if (user->initialize(userId, userInfo) == false)
    {
        ErrorLog("user Initialize error");
        return INTERNAL_ERROR;
    }
    
    if (userIdMap.insert(std::map<std::string, User*>::value_type(userId, user)).second == false)
    {
        ErrorLog("user insert fail ");
        delete user;
        return ALREADY_EXIST_USER;
    }

    std::string str(user->nickName, user->nickNameLen);

    if (userMap.insert(std::map<std::string, User*>::value_type(str, user)).second == false)
    {
        ErrorLog("user insert fail");
        userIdMap.erase(userId);
        delete user;
        return ALREADY_EXIST_USER;
    }

    
    if (userNoMap.insert(std::map<int64_t, User*>::value_type(user->userNo, user)).second == false)
    {
        ErrorLog("user insert fail");
        userIdMap.erase(userId);
        userMap.erase(str);
        delete user;
        return ALREADY_EXIST_USER;
    }

    
    return SUCCESS;
}


User* UserManager::getUserByUserId(std::string userId)
{
    std::map< std::string, User* >::const_iterator iter = userIdMap.find(userId);
    
    if (iter == userIdMap.end())
    {
        ErrorLog("not exist User");
        return NULL;
    }
    
    return iter->second;
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
    std::map< int64_t, User* >::const_iterator iter = userNoMap.find(userNo);
    
    if (iter == userNoMap.end())
    {
        ErrorLog("not exist User");
        return NULL;
    }
    
    return iter->second;
}


bool UserManager::removeUser(User* user)
{
    if (userMap.erase(std::string(user->nickName, user->nickNameLen)) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    if (userIdMap.erase(user->userId) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    if (userNoMap.erase(user->userNo) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    user->logoutUser();
    
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
    
    if (userMap.erase(std::string(user->nickName, user->nickNameLen)) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    if (userIdMap.erase(user->userId) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    if (userNoMap.erase(user->userNo) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    user->logoutUser();
    
    delete (user);
    
    return true;
}

bool UserManager::setFriend(User* user1, User* user2)
{
    if(user1->addFriend(user2) == false)
    {
        ErrorLog("??");
        return false;
    }
    
    if(user2->addFriend(user1) == false)
    {
        ErrorLog("??");
        user1->removeFriend(user2);
        return false;
    }
    
    return true;
}


bool UserManager::removeFriend(User* user1, User* user2)
{
    if(user1->removeFriend(user2) == false)
    {
        ErrorLog("??");
        return false;
    }
    
    if(user2->removeFriend(user1) == false)
    {
        ErrorLog("??");
        user1->addFriend(user2);
        return false;
    }
    
    return true;
}



