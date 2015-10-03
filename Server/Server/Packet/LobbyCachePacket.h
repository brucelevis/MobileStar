#ifndef __LOBBY_CACHE_PACKET_H__
#define __LOBBY_CACHE_PACKET_H__

#include "BasicPacket.h"
#include "NetworkDefines.h"

class LobbyCachePacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;


	enum EnumCommand // start number is at least 3.
	{
		PACKET_VERSION_REQ = 10,
		PACKET_VERSION_RES,
		PACKET_VERSION_FAIL,

		LOGIN_USER_REQ,
		LOGIN_USER_RES,
		LOGIN_USER_FAIL,

		LOGOUT_USER_REQ,
		LOGOUT_USER_RES,
		LOGOUT_USER_FAIL,

		GET_USER_INFO_REQ,
		GET_USER_INFO_RES,
		GET_USER_INFO_FAIL,

		GET_FRIEND_LIST_REQ,
		GET_FRIEND_LIST_RES,
		GET_FRIEND_LIST_FAIL,

		ADD_REQUEST_FRIEND_REQ,
		ADD_REQUEST_FRIEND_RES,
		ADD_REQUEST_FRIEND_FAIL,
		ADD_REQUEST_FRIEND_NOTIFY,

		ADD_RESPONSE_FRIEND_REQ,
		ADD_RESPONSE_FRIEND_RES,
		ADD_RESPONSE_FRIEND_FAIL,

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


	struct PacketVersionReqPacket
	{
		int32_t packetVersion;
	};

	struct PacketVersionResPacket
	{

	};

	struct LoginUserReqPacket
	{
		SessionId_t sid;

		int64_t userNo;
	};

	struct LoginUserResPacket
	{
		SessionId_t sid;

		int16_t requestInfoCount;
		RequestInfo requestInfoList[MAX_REQUEST_INFO_COUNT];
	};

	struct LogoutUserReqPacket
	{
		int64_t userNo;
	};

	struct LogoutUserResPacket
	{
		int64_t userNo;
	};

	struct GetUserInfoReqPacket
	{
		SessionId_t sid;

		NickNameInfo nickNameInfo;
	};

	struct GetUserInfoResPacekt
	{
		SessionId_t sid;

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
		SessionId_t sid;

		int64_t userNo;
	};

	struct GetFriendListResPacket
	{
		SessionId_t sid;

		int16_t nickNameInfoWithOnlineListCount;
		NickNameInfoWithOnline NickNameInfoWithOnlineList[MAX_FRIEND_COUNT];
	};

	struct AddRequestFriendReqPacket
	{
		SessionId_t sid;

		int64_t userNo;

		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN + 1];
	};

	struct AddRequestFriendResPacket
	{
		SessionId_t sid;
	};

	struct AddRequestFriendNotifyPacket
	{
		int64_t userNo;

		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN + 1];
	};

	struct AddResponseFriendReqPacket
	{
		SessionId_t sid;

		int64_t userNo;

		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN + 1];

		int8_t isAccepted;
	};

	struct AddResponseFriendResPacket
	{
		SessionId_t sid;
	};

	struct RemoveFriendReqPacket
	{
		SessionId_t sid;

		int64_t userNo;

		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN + 1];
	};

	struct RemoveFriendResPacket
	{
		SessionId_t sid;
	};

	struct FailPacket
	{
		SessionId_t sid;

		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__LOBBY_CACHE_PACKET_H__
