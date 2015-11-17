#ifndef __LOBBY_CACHE_PACKET_H__
#define __LOBBY_CACHE_PACKET_H__

#include "BasicPacket.h"

class LobbyCachePacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;


	enum EnumCommand // start number is at least 3.
	{
        FIRST_CONNECT_REQ = 10,
        FIRST_CONNECT_RES,
        FIRST_CONNECT_FAIL,
        
        LOGIN_USER_REQ,
        LOGIN_USER_RES,
        LOGIN_USER_FAIL,

		GET_USER_INFO_REQ,
		GET_USER_INFO_RES,
		GET_USER_INFO_FAIL,
        
        GET_FRIEND_LIST_REQ,
        GET_FRIEND_LIST_RES,
        GET_FRIEND_LIST_FAIL,
		
        GET_CLAN_USER_LIST_REQ,
        GET_CLAN_USER_LIST_RES,
        GET_CLAN_USER_LIST_FAIL,
        
        ADD_FRIEND_REQ,
        ADD_FRIEND_RES,
        ADD_FRIEND_FAIL,
        
		REMOVE_FRIEND_REQ,
		REMOVE_FRIEND_RES,
		REMOVE_FRIEND_FAIL,
        
        
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
		ALREADY_REQUEST,
		ALREADY_FRIEND,
		NOT_FRIEND,
		ROOM_IS_FULL,
		ALL_ROOM_SLOT_IS_FULL,
		P_ALL_USER_IS_NOT_READY,
		P_INCORRECT_SOLVE_DELAY,
		P_ALREADY_FINISH_GAME,
	};

	///////////////////////////////////

#pragma pack(push, 1)

	///////////////////////////////////


    struct FirstConnectReqPacket : public Packet
    {
        FirstConnectReqPacket() { cmd = FIRST_CONNECT_REQ; }
    };
    
    struct FirstConnectResPacket : public Packet
    {
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }
        
    };

	struct LoginUserReqPacket : public Packet
	{
        LoginUserReqPacket() { cmd = LOGIN_USER_REQ; }
        
		int64_t userNo;
	};

	struct LoginUserResPacket : public Packet
	{
        LoginUserResPacket() { cmd = LOGIN_USER_RES; }
        
        UserInfo userInfo;
        
        int8_t friendCount;
        NickNameInfo nickNameInfoList[MAX_FRIEND_COUNT];
        
        ClanInfo clanInfo;
	};

	struct LogoutUserReqPacket
	{
        NickNameInfo myNickNameInfo;
	};

	struct LogoutUserResPacket
	{
        NickNameInfo myNickNameInfo;
	};

	struct GetUserInfoReqPacket
	{
        NickNameInfo myNickNameInfo;

		NickNameInfo nickNameInfo;
	};

	struct GetUserInfoResPacekt
	{
        NickNameInfo myNickNameInfo;

		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN + 1];
		int32_t gold;
		int8_t memoLen;
		char memo[MAX_MEMO_LEN + 1];
		int16_t commonWin;
		int16_t commonLose;
		int16_t commonDiss;
		int16_t rankWin;
		int16_t rankLose;
		int16_t rankDiss;
		int8_t grade;
		int8_t gradeReachedCount;
		int8_t point;
		int8_t icon;
	};

	struct GetFriendListReqPacket
	{
        NickNameInfo myNickNameInfo;
	};

	struct GetFriendListResPacket
	{
        NickNameInfo myNickNameInfo;
        
        int8_t friendCount;
        NickNameInfo nickNameInfoList[MAX_FRIEND_COUNT];
	};

	struct AddRequestFriendReqPacket
	{
        NickNameInfo myNickNameInfo;

        NickNameInfo nickNameInfo;
    };

	struct AddRequestFriendResPacket
	{
        NickNameInfo myNickNameInfo;
    };

	struct AddFriendReqPacket
	{
        NickNameInfo myNickNameInfo;
        
        NickNameInfo nickNameInfo;
	};

	struct AddFriendResPacket
	{
        NickNameInfo myNickNameInfo;
	};

	struct RemoveFriendReqPacket
	{
        NickNameInfo myNickNameInfo;

        NickNameInfo nickNameInfo;
    };

	struct RemoveFriendResPacket
	{
        NickNameInfo myNickNameInfo;
    };

	struct FailPacket
	{
		SessionId_t sid;

		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__LOBBY_CACHE_PACKET_H__
