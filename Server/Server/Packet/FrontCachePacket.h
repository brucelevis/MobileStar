#ifndef __FRONT_CACHE_PACKET_H__
#define __FRONT_CACHE_PACKET_H__

#include "BasicPacket.h"
#include "NetworkDefines.h"

class FrontCachePacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;

	enum EnumCommand // start number is at least 3.
	{
		PACKET_VERSION_REQ = 10,
		PACKET_VERSION_RES,
		PACKET_VERSION_FAIL,

		CREATE_USER_REQ,
		CREATE_USER_RES,
		CREATE_USER_FAIL,

		LOGIN_USER_REQ,
		LOGIN_USER_RES,
		LOGIN_USER_FAIL,

		LOGOUT_USER_REQ,

		DELETE_USER_REQ,
		DELETE_USER_RES,
		DELETE_USER_FAIL,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
		EXIST_EMAIL,
		EXIST_NICK_NAME,
		NOT_EXIST_EMAIL,
		PASSWORD_INCORRECT,
	};

	///////////////////////////////////

	struct UserInfo
	{
		int64_t userNo;
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

#pragma pack(push, 1)

	///////////////////////////////////


	struct PacketVersionReqPacket
	{
		int32_t packetVersion;
	};

	struct PacketVersionResPacket
	{

	};

	struct CreateUserReqPacket
	{
		SessionId_t sid;

		int8_t emailLen;
		char email[MAX_EMAIL_LEN + 1];
		int8_t passwordLen;
		char password[MAX_PASSWORD_LEN + 1];
		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN + 1];
	};

	struct CreateUserResPacket
	{
		SessionId_t sid;
	};

	struct LoginUserReqPacket
	{
		SessionId_t sid;

		int8_t emailLen;
		char email[MAX_EMAIL_LEN + 1];
		int8_t passwordLen;
		char password[MAX_PASSWORD_LEN + 1];
	};

	struct LoginUserResPacket
	{
		SessionId_t sid;

		int64_t userNo;
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

	struct LogoutUserReqPacket
	{
		int64_t userNo;
	};

	struct DeleteUserReqPacket
	{
		SessionId_t sid;

		int64_t userNo;
	};

	struct DeleteUserResPacket
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
#endif //__FRONT_CACHE_PACKET_H__
