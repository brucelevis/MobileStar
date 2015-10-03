#ifndef __LOGIN_FRONT_PACKET_H__
#define __LOGIN_FRONT_PACKET_H__

#include "BasicPacket.h"
#include "NetworkDefines.h"

class LobbyCenterPacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;
	
	enum EnumCommand // start number is at least 3.
	{
		FIRST_CONNECT_REQ = 10,
		FIRST_CONNECT_RES,
		FIRST_CONNECT_FAIL,

		ENTER_FRONT_USER_REQ,
		ENTER_FRONT_USER_RES,
		ENTER_FRONT_USER_FAIL,

		GET_CHANNEL_LIST_REQ,
		GET_CHANNEL_LIST_RES,
		GET_CHANNEL_LIST_FAIL,

		MOVE_CHANNEL_REQ,
		MOVE_CHANNEL_RES,
		MOVE_CHANNEL_FAIL,

		LOGOUT_USER_REQ,
		LOGOUT_USER_RES,
		LOGOUT_USER_FAIL,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
	};

	///////////////////////////////////

#pragma pack(push, 1)

	///////////////////////////////////


	struct FirstConnectReqPacket
	{
		int32_t packetVersion;

		int8_t channelCount;
		BasicPacket::ChannelInfo channelInfo[BasicPacket::MAX_CHANNEL_COUNT];
	};

	struct FirstConnectResPacket
	{
		int8_t channelCount;
		BasicPacket::ChannelInfo channelInfo[BasicPacket::MAX_CHANNEL_COUNT];
	};

	struct EnterFrontUserReqPacket
	{
		SessionId_t sid;

		int16_t channelNo;
		int64_t userNo;
		int8_t nickNameLen;
		char nickName[BasicPacket::MAX_NICK_NAME_LEN + 1];
		int32_t gold;
		int8_t memoLen;
		char memo[BasicPacket::MAX_MEMO_LEN + 1];
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

	struct EnterFrontUserResPacket
	{
		SessionId_t sid;
	};

	struct GetChannelListReqPacket
	{
		SessionId_t sid;
	};

	struct GetChannelListResPacket
	{
		SessionId_t sid;

		int8_t channelCount;
		BasicPacket::ChannelInfo channelInfo[BasicPacket::MAX_CHANNEL_COUNT];
	};

	struct MoveChannelReqPacket
	{
		SessionId_t sid;

		int64_t userNo;
		int16_t channelNo;
	};

	struct MoveChannelResPacket
	{
		SessionId_t sid;

		int16_t channelNo;
	};

	struct LogoutUserReqPacket
	{
		int64_t userNo;
	};

	struct LogoutUserResPacket
	{
		int64_t userNo;
	};

	struct FailPacket
	{
		SessionId_t sid;

		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__LOBBY_CENTER_PACKET_H__
