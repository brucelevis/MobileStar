//
//  GameDBChannel.h
//  CacheServer
//
//  Created by jooyc on 14. 1. 25..
//  Copyright (c) 2014년 Team ORIYEL. All rights reserved.
//

#ifndef __GAME_DB_CHANNEL_H__
#define __GAME_DB_CHANNEL_H__

#include "DBConn.h"
#include <string>
#include "BasicPacket.h"

class ClanManager;
class UserManager;
class UserInfo;

class GameDBChannel
{
public:
    bool Initialize(const char* host, const char* user, const char* pwd, const char* db, const int port);
	bool InitUserInMemory(ClanManager* clanMgr, UserManager* userMgr);

    int64_t InsertUser(std::string userId, std::string nickName);
	bool SelectUser(int64_t userNo, UserInfo* userInfo);
    void SelectFriendList(int64_t userNo, int* friendCount, NickNameInfo* nickNameInfoList);
	bool Logout(int64_t userNo, int32_t lastLogin, int32_t lastLogout, const char* memo, int8_t icon);
	bool DeleteUser(int64_t userNo);
private:
    DBConn _dbConn;
};

#endif

