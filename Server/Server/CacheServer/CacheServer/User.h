
// create by yong chan kim
// since 2014.7.29


#ifndef __USER_H__
#define __USER_H__

#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "BasicPacket.h"

struct UserLoginInfo
{
    int32_t gold;
    int8_t memoLen;
    char memo[MAX_MEMO_LEN];
    int16_t commonWin;
    int16_t commonLose;
    int16_t commonDiss;
    int16_t rankWin;
    int16_t rankLose;
    int16_t rankDiss;
    int8_t grade;
    int8_t gradeReachedCount;
};


class ConnectInfo;


class User
{
public:

	User();

    bool initialize(std::string _userId, UserInfo* userInfo);
    
    int loginUser(UserInfo* userInfo);
    int logoutUser();
    
    bool addFriend(User* user);
    bool removeFriend(User* user);
    
	int64_t getUserNo() { return userNo; }
	void setUserNo(int64_t _userNo) { userNo = _userNo; }

	const char* getNickName() { return nickName; }
	int8_t getNickNameLen() { return nickNameLen; }
	void setNickName(char* _nickName, int8_t _nickNameLen)
	{
		nickNameLen = _nickNameLen;
		memcpy(nickName, _nickName, nickNameLen);
	}
    
    void getUserInfo(UserInfo* userInfo);
    int getFriendList(NickNameInfo* nickNameInfo);
    
    int32_t getClanNo() { return clanNo; }
    
	~User();

public:

	friend class UserManager;

private:
    std::string userId;
    int64_t userNo;
    int8_t nickNameLen;
    char nickName[MAX_NICK_NAME_LEN];
    int16_t commonWin;
    int16_t commonLose;
    int16_t commonDiss;
    int16_t rankWin;
    int16_t rankLose;
    int16_t rankDiss;
    int8_t grade;
    int8_t gradeReachedCount;
    int32_t point;
    int32_t coin;
    int32_t clanNo;
    int8_t clanClass;
    
    std::vector<User*> friendList;
};

#include <string.h>
#include <stdint.h>
#include "BasicPacket.h"
#include <vector>

#include <deque>
#include <map>

class UserManager
{
public:
    UserManager();
    
    bool initialize();
    
    int addUser(std::string userId, UserInfo* userInfo);
    
    User* getUserByUserId(std::string userId);
    User* getUserByNickNameInfo(NickNameInfo* nickNameInfo);
    User* getUserByUserNo(int64_t userNo);
    
    bool removeUser(User* _user);
    bool removeUserByNickNameInfo(NickNameInfo* nickNameInfo);
    
    bool setFriend(User* user1, User* user2);
    bool removeFriend(User* user1, User* user2);
    
    ~UserManager();
    
private:
    std::map<std::string, User*> userIdMap;
    std::map<std::string, User*> userMap;
    std::map<int64_t, User*> userNoMap;
};

#endif //__USER_H__