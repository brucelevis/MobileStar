//
//  GameDBChannel.cpp
//  CacheServer
//
//  Created by jooyc on 14. 1. 25..
//  Copyright (c) 2014년 Team ORIYEL. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>

#include "GameDBChannel.h"
#include "Log.h"
#include "User.h"
#include "Clan.h"


#define TRAN_BEGIN      "BEGIN;"
#define TRAN_COMMIT     "COMMIT;"



///////////////use
#define SELECT_ALL_USER_QUERY      "SELECT * FROM User;"

#define INSERT_USER_QUERY   "INSERT INTO User (UserId, NickName, Coin, Grade, CreateTime, LastLoginTime, LastLogoutTime) VALUES('%s', '%s', %d, %d, NOW(), NOW(), NOW());"

#define SELECT_USER_QUERY      "SELECT * FROM User WHERE UserNo = %lld;"

#define SELECT_FRIEND_QUERY "SELECT u.NickName n1, u.nickName n2 FROM User u, Friend f WHERE f.F1 = %lld or f.F2 = %lld and u.UserNo = f.F1 or u.userNo = f.F2;"


#define UPDATE_LOGOUT_QUERY			"UPDATE User SET LastLogin = FROM_UNIXTIME(%d), LastLogout = FROM_UNIXTIME(%d), Memo = '%s', Icon = %d WHERE UserNo = %lld;"

#define UPDATE_DELETE_USER_QUERY	"UPDATE User SET Deletion = 1 WHERE UserNo = %lld;"

#define SELECT_ALL_CLAN_QUERY "SELECT * FROM Clan;"

#define SELECT_ALL_FRIEND_QUERY "SELECT * FROM Friend;"


/////////////////not use
//// Insert Query

#define INSERT_STUDENT_QUERY   "INSERT INTO Student (UserNo, Type, Level, Created) \
    VALUES(%lld, %d, %d, NOW());"


//// Select Query


#define SELECT_USER_QUERY_BY_USER_NO    "SELECT UserNo, UserId, UserName, Email, Password, Level, Exp, SelectedCharType, Gold, Diamond, TopScore, Created, UNIX_TIMESTAMP(LastLogin), UNIX_TIMESTAMP(LastLogout), WinMsg, TopScoreCharType, Grade, GradeReachedCount, BestScore, Energy, Deletion FROM User WHERE UserNo = %lld and Deletion = 0;"

#define SELECT_ALL_USER_STUDENT_QUERY   "SELECT * FROM Student where UserNo in (select UserNo from User where Deletion = 0);"

#define SELECT_ALL_STUDENT_QUERY   "SELECT * FROM Student WHERE UserNo = %lld;"

#define SELECT_STUDENT_QUERY   "SELECT * FROM Student WHERE UserNo = %lld and Type = %d;"

#define SELECT_USER_ACCOUNT_INFO_QUERY      "SELECT UserNo FROM User WHERE Email like N'%s' and Password like N'%s';"

#define SELECT_USER_GOLD_AND_DIAMOND_QUERY  "SELECT Gold, Diamond FROM User WHERE UserNo = %lld;"

//// Update Query
#define UPDATE_USER_ID_QUERY              "UPDATE User SET UserId = '%s' WHERE UserNo = %lld;"

#define UPDATE_USER_GOLD_QUERY      "UPDATE User SET Gold = Gold + %d WHERE UserNo = %lld;"

#define UPDATE_EXP_QUERY            "UPDATE User SET Exp = Exp + %d WHERE UserNo = %lld;"

#define UPDATE_USER_REWARD      "UPDATE User SET Gold = %d, Exp = %d, Level = %d, Grade = %d, GradeReachedCount = %d, TopScore = %d, TopScoreCharType = %d, BestScore = %d WHERE UserNo = %lld;"

#define UPDATE_CHARACTER_LEVEL_UP_QUERY     "\
    UPDATE Student SET Level = Level + 1 \
                   WHERE UserNo = %lld AND Type = %d;"

#define UPDATE_USER_PAY_GOLD_OR_DIAMOND_QUERY   "UPDATE User SET Gold = %d, Diamond = %d WHERE UserNo = %lld;"

#define UPDATE_WIN_MESSAGE_QUERY	"UPDATE User SET WinMsg = '%s' WHERE UserNo = %lld;"

#define UPDATE_LOGIN_QUERY			"UPDATE User SET LastLogin = FROM_UNIXTIME(%d) WHERE UserNo = %lld;"

#define INSERT_PURCHASE_QUERY   "INSERT INTO Purchase (UserNo, PurchaseToken, ProductId, State, Created, Finished, ResultCode) \
    VALUES(%lld, '%s', '%s', %d, NOW(), NOW(), %d);"

#define SELECT_PURCHASE_QUERY   "SELECT * FROM Purchase WHERE UserNo = %lld and PurchaseToken like N'%s' and State = %d;"

#define UPDATE_PURCHASE_QUERY   "UPDATE Purchase SET State = %d, ResultCode = %d, Finished = Now() Where UserNo = %lld and PurchaseToken like N'%s' and State = %d;"

#define UPDATE_DIAMOND_QUERY		"UPDATE User Set Diamond = Diamond + %d WHERE UserNo = %lld;"

#define UPDATE_EXCHANGE_DIAMOND_QUERY		"UPDATE User Set Gold = Gold + %d, Diamond = Diamond - %d WHERE UserNo = %lld;"

enum CLAN_FIELD_NAME
{
    CLAN_CLAN_NO = 0,
    CLAN_CLAN_NAME,
    CLAN_WIN,
    CLAN_LOSE,
    CLAN_POINT,
    CLAN_CREATE_TIME,
    
    CLAN_FIELD_COUNT,
};

enum USER_FIELD_NAME {
    USER_USER_NO = 0,
    USER_USER_ID,
    USER_NICK_NAME,
	USER_COMMON_WIN,
	USER_COMMON_LOSE,
	USER_COMMON_DISS,
	USER_RANK_WIN,
	USER_RANK_LOSE,
	USER_RANK_DISS,
	USER_GRADE,
	USER_GRADE_REACHED_COUNT,
	USER_POINT,
	USER_COIN,
    USER_CLAN_NO,
    USER_CLAN_CLASS,
    USER_CREATED_TIME,
    USER_LAST_LOGIN_TIME,
    USER_LAST_LOGOUT_TIME,

    USER_FIELD_COUNT
};

enum FRIEND_FIELD_NAME
{
    FRIEND_F1 = 0,
    FRIEND_F2,
    
    FRIEND_FIELD_COUNT,
};

static const int32_t DEFAULT_COIN = 0;
static const int8_t DEFAULT_GRADE = 0;

bool GameDBChannel::Initialize(const char* host, const char* user, const char* pwd, const char* db, const int port)
{
    if (!_dbConn.Initialize(host, user, pwd, db, port))
        return false;

    return true;
}

bool GameDBChannel::InitUserInMemory(ClanManager* clanMgr, UserManager* userMgr)
{
    char query[1024] = { 0, };
    sprintf(query,
            SELECT_ALL_CLAN_QUERY);
    
    MYSQL_RES* resultSet = NULL;
    _dbConn.ProcessResultQuery(query, &resultSet);
    
    
    
    if (resultSet)
    {
        MYSQL_ROW row;
        
        ClanInfo clanInfo;
        
        while ((row = mysql_fetch_row(resultSet)) != NULL)
        {
            mysql_field_seek(resultSet, 0);
            
            if (mysql_num_fields(resultSet) != CLAN_FIELD_COUNT)
            {
                ErrorLog("the retrieved field is incorrect, fields:%d", mysql_num_fields(resultSet));
                mysql_free_result(resultSet);
                return false;
            }
            
            memset(&clanInfo, 0, sizeof(ClanInfo));
            
            if (row[CLAN_CLAN_NO]) clanInfo.clanNo = atoi(row[CLAN_CLAN_NO]);
            if (row[CLAN_CLAN_NAME])
            {
                strcpy(clanInfo.clanName, row[CLAN_CLAN_NAME]);
                clanInfo.clanNameLen = strlen(row[CLAN_CLAN_NAME]);
            }
            if (row[CLAN_WIN]) clanInfo.win = atoi(row[CLAN_WIN]);
            if (row[CLAN_LOSE]) clanInfo.lose = atoi(row[CLAN_LOSE]);
            if (row[CLAN_POINT]) clanInfo.point = atoi(row[CLAN_POINT]);

            if (clanMgr->addClan(&clanInfo) != SUCCESS)
            {
                ErrorLog("AddClan error");
                mysql_free_result(resultSet);
                return false;
            }
        }
        
        mysql_free_result(resultSet);
    }
    
    char query1[1024] = { 0, };
    sprintf(query1,
            SELECT_ALL_USER_QUERY);

    resultSet = NULL;
    _dbConn.ProcessResultQuery(query1, &resultSet);

    if (resultSet)
    {
        MYSQL_ROW row;

		UserInfo userInfo;
        
        while ((row = mysql_fetch_row(resultSet)) != NULL)
        {
            mysql_field_seek(resultSet, 0);

            if (mysql_num_fields(resultSet) != USER_FIELD_COUNT)
            {
                ErrorLog("the retrieved field is incorrect, fields:%d", mysql_num_fields(resultSet));
                mysql_free_result(resultSet);
                return false;
            }

            memset(&userInfo, 0, sizeof(UserInfo));
            
            if (row[USER_USER_NO]) userInfo.userNo = atoll(row[USER_USER_NO]);
            if (row[USER_NICK_NAME])
            {
                strcpy(userInfo.nickName, row[USER_NICK_NAME]);
                userInfo.nickNameLen = strlen(row[USER_NICK_NAME]);
            }
            if (row[USER_COMMON_WIN]) userInfo.commonWin = atoi(row[USER_COMMON_WIN]);
            if (row[USER_COMMON_LOSE]) userInfo.commonLose = atoi(row[USER_COMMON_LOSE]);
            if (row[USER_COMMON_DISS]) userInfo.commonDiss = atoi(row[USER_COMMON_DISS]);
            if (row[USER_RANK_WIN]) userInfo.rankWin = atoi(row[USER_RANK_WIN]);
            if (row[USER_RANK_LOSE]) userInfo.rankLose = atoi(row[USER_RANK_LOSE]);
            if (row[USER_RANK_DISS]) userInfo.rankDiss = atoi(row[USER_RANK_DISS]);
            if (row[USER_GRADE]) userInfo.grade = atoi(row[USER_GRADE]);
            if (row[USER_GRADE_REACHED_COUNT]) userInfo.gradeReachedCount = atoi(row[USER_GRADE_REACHED_COUNT]);
            if (row[USER_POINT]) userInfo.point = atoi(row[USER_POINT]);
            if (row[USER_COIN]) userInfo.coin = atoi(row[USER_COIN]);
            if (row[USER_CLAN_NO]) userInfo.clanNo = atoi(row[USER_CLAN_NO]);
            if (row[USER_CLAN_CLASS]) userInfo.clanClass = atoi(row[USER_CLAN_CLASS]);

            if (userMgr->addUser(std::string(row[USER_USER_ID], strlen(row[USER_USER_ID])), &userInfo) != SUCCESS)
            {
                ErrorLog("AddUser error");
                mysql_free_result(resultSet);
                return false;
            }

            char userId[100] = {0, };
            strcpy(userId, row[USER_USER_ID]);
            
            User* user = userMgr->getUserByUserId(std::string(row[USER_USER_ID], strlen(row[USER_USER_ID])));

            if (user == NULL)
            {
                ErrorLog("user is null ");
                mysql_free_result(resultSet);
                return false;
            }
            if(userInfo.clanNo != 0)
            {
                Clan* clan = clanMgr->getClanByClanNo(userInfo.clanNo);
                
                if(clan == NULL)
                {
                    ErrorLog("clan is null");
                    mysql_free_result(resultSet);
                    return false;
                }
                if(clan->addUser(user) != SUCCESS)
                {
                    ErrorLog("adduser error");
                    mysql_free_result(resultSet);
                    return false;
                }
            }
        }

        mysql_free_result(resultSet);
    }
    
    char query2[1024] = { 0, };
    sprintf(query2,
            SELECT_ALL_FRIEND_QUERY);
    
    resultSet = NULL;
    _dbConn.ProcessResultQuery(query2, &resultSet);
    
    if (resultSet)
    {
        MYSQL_ROW row;
        
        User* user1 = NULL;
        User* user2 = NULL;

        while ((row = mysql_fetch_row(resultSet)) != NULL)
        {
            mysql_field_seek(resultSet, 0);
            
            if (mysql_num_fields(resultSet) != FRIEND_FIELD_COUNT)
            {
                ErrorLog("the retrieved field is incorrect, fields:%d", mysql_num_fields(resultSet));
                mysql_free_result(resultSet);
                return false;
            }
            
            if (row[FRIEND_F1])
            {
                user1 = userMgr->getUserByUserNo(atoll(row[FRIEND_F1]));
                if(user1 == NULL)
                {
                    ErrorLog("getUserByUserNo error");
                    mysql_free_result(resultSet);
                    return false;
                }
            }
            
            if (row[FRIEND_F2])
            {
                user2 = userMgr->getUserByUserNo(atoll(row[FRIEND_F2]));
                if(user2 == NULL)
                {
                    ErrorLog("getUserByUserNo error");
                    mysql_free_result(resultSet);
                    return false;
                }
            }
            
            if(userMgr->setFriend(user1, user2) == false)
            {
                ErrorLog("setFriend error");
                mysql_free_result(resultSet);
                return false;
            }
        }
        
        mysql_free_result(resultSet);
    }


    return true;
}

int64_t GameDBChannel::InsertUser(std::string userId, std::string nickName)
{
    char query[1024] = { 0, };
    
    sprintf(query,
            INSERT_USER_QUERY,
			userId.c_str(), nickName.c_str(), DEFAULT_COIN, DEFAULT_GRADE);

	unsigned long long userNo = 0;
	if (!_dbConn.ProcessNoResultQuery(query, &userNo))
		return -1;

	return (int64_t)userNo;

}

bool GameDBChannel::SelectUser(int64_t userNo, UserInfo* userInfo)
{
	char query[256] = { 0, };

	sprintf(query,
		SELECT_USER_QUERY,
		userNo);

	MYSQL_RES* resultSet = NULL;
	_dbConn.ProcessResultQuery(query, &resultSet);

	bool isExists = false;

	if (resultSet)
	{
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(resultSet)) != NULL)
		{
			mysql_field_seek(resultSet, 0);

			if (mysql_num_fields(resultSet) != USER_FIELD_COUNT)
			{
				ErrorLog("the retrieved field is incorrect, fields:%d", mysql_num_fields(resultSet));
				mysql_free_result(resultSet);
				return false;
			}
            
            memset(userInfo, 0, sizeof(UserInfo));
            
            if (row[USER_USER_NO]) userInfo->userNo = atoll(row[USER_USER_NO]);
            if (row[USER_NICK_NAME])
            {
                strcpy(userInfo->nickName, row[USER_NICK_NAME]);
                userInfo->nickNameLen = strlen(row[USER_NICK_NAME]);
            }
			if (row[USER_COMMON_WIN]) userInfo->commonWin = atoi(row[USER_COMMON_WIN]);
			if (row[USER_COMMON_LOSE]) userInfo->commonLose = atoi(row[USER_COMMON_LOSE]);
			if (row[USER_COMMON_DISS]) userInfo->commonDiss = atoi(row[USER_COMMON_DISS]);
			if (row[USER_RANK_WIN]) userInfo->rankWin = atoi(row[USER_RANK_WIN]);
			if (row[USER_RANK_LOSE]) userInfo->rankLose = atoi(row[USER_RANK_LOSE]);
			if (row[USER_RANK_DISS]) userInfo->rankDiss = atoi(row[USER_RANK_DISS]);
			if (row[USER_GRADE]) userInfo->grade = atoi(row[USER_GRADE]);
			if (row[USER_GRADE_REACHED_COUNT]) userInfo->gradeReachedCount = atoi(row[USER_GRADE_REACHED_COUNT]);
			if (row[USER_POINT]) userInfo->point = atoi(row[USER_POINT]);
			if (row[USER_COIN]) userInfo->coin = atoi(row[USER_COIN]);
            if (row[USER_CLAN_NO]) userInfo->clanNo = atoi(row[USER_CLAN_NO]);
            if (row[USER_CLAN_CLASS]) userInfo->clanClass = atoi(row[USER_CLAN_CLASS]);
            
//          if (row[USER_CREATED_TIME]) userInfo->createTime = atoi(row[USER_CREATED_TIME]);
//			if (row[USER_LAST_LOGIN_TIME]) userInfo->lastLoginTime = atoi(row[USER_LAST_LOGIN_TIME]);
//			if (row[USER_LAST_LOGOUT_TIME]) userInfo->lastLogoutTime = atoi(row[USER_LAST_LOGOUT_TIME]);

			isExists = true;
		}

		mysql_free_result(resultSet);
	}

	if (false == isExists)
	{
		ErrorLog("the user do not exist, userNo = %lld", userNo);
		return false;
	}

	return true;
}


void GameDBChannel::SelectFriendList(int64_t userNo, int* friendCount, NickNameInfo* nickNameInfoList)
{
    char query[256] = { 0, };
    
    sprintf(query,
            SELECT_FRIEND_QUERY,
            userNo, userNo);
    
    
    MYSQL_RES* resultSet = NULL;
    _dbConn.ProcessResultQuery(query, &resultSet);
    
    if (resultSet)
    {
        MYSQL_ROW row;
        
        int i = 0;
        
        while ((row = mysql_fetch_row(resultSet)) != NULL)
        {
            mysql_field_seek(resultSet, 0);
            
            if (mysql_num_fields(resultSet) != USER_FIELD_COUNT)
            {
                ErrorLog("the retrieved field is incorrect, fields:%d", mysql_num_fields(resultSet));
                mysql_free_result(resultSet);
                return ;
            }
            
            if (atoll(row[FRIEND_F1]) == userNo)
            {
                strcpy(nickNameInfoList[i].nickName, row[FRIEND_F1]);
                nickNameInfoList[i].nickNameLen = strlen(row[FRIEND_F1]);
            }
            else if (atoll(row[FRIEND_F2]) == userNo)
            {
                strcpy(nickNameInfoList[i].nickName, row[FRIEND_F2]);
                nickNameInfoList[i].nickNameLen = strlen(row[FRIEND_F2]);
            }
            else
            {
                ErrorLog("??");
            }
            
            i++;
        }
        
        *friendCount = i;
        
        mysql_free_result(resultSet);
    }
}


bool GameDBChannel::Logout(int64_t userNo, int32_t lastLogin, int32_t lastLogout, const char* memo, int8_t icon)
{
	char query[256] = { 0, };

	sprintf(query,
		UPDATE_LOGOUT_QUERY,
		lastLogout, lastLogout, memo, icon, userNo);

	if (false == _dbConn.ProcessNoResultQuery(query))
		return false;

	return true;
}

bool GameDBChannel::DeleteUser(int64_t userNo)
{
	char query[256] = { 0, };

	sprintf(query,
		UPDATE_DELETE_USER_QUERY,
		userNo);

	if (false == _dbConn.ProcessNoResultQuery(query))
		return false;

	return true;
}

