#ifndef __GAME_CACHE_PACKET_H__
#define __GAME_CACHE_PACKET_H__

#include "BasicPacket.h"

class GameCachePacket
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

		CREATE_ROOM_REQ,
		CREATE_ROOM_RES,
		CREATE_ROOM_FAIL,

		JOIN_ROOM_REQ,
		JOIN_ROOM_RES,
		JOIN_ROOM_FAIL,

		RANDOM_JOIN_ROOM_REQ,
		RANDOM_JOIN_ROOM_RES,
		RANDOM_JOIN_ROOM_FAIL,

		JOIN_ROOM_NOTIFY,

		READY_REQ,
		READY_RES,
		READY_FAIL,

		READY_NOTIFY,

		UNREADY_REQ,
		UNREADY_RES,
		UNREADY_FAIL,

		UNREADY_NOTIFY,

		LEAVE_ROOM_REQ,
		LEAVE_ROOM_RES,
		LEAVE_ROOM_FAIL,

		LEAVE_ROOM_NOTIFY,

		START_GAME_NOTIFY,

		MOVE_REQ,
		MOVE_NOTIFY,
		MOVE_FAIL,

		FINISH_GAME_NOTIFY,

		LOGOUT_USER_REQ,
		LOGOUT_USER_RES,
		LOGOUT_USER_FAIL,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
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

	struct FailPacket
	{
		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__GAME_CACHE_PACKET_H__
