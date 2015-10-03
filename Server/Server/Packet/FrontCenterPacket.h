#ifndef __FRONT_CENTER_PACKET_H__
#define __FRONT_CENTER_PACKET_H__

#include "BasicPacket.h"
#include "NetworkDefines.h"

class FrontCenterPacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;

	enum EnumCommand // start number is at least 3.
	{
		PACKET_VERSION_REQ = 10,
		PACKET_VERSION_RES,
		PACKET_VERSION_FAIL,

		LOBBY_SERVER_NO_REQ,
		LOBBY_SERVER_NO_RES,
		LOBBY_SERVER_NO_FAIL,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
		ALREADY_EXIST_USER,
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

	struct LobbyServerNoReqPacket
	{
		SessionId_t sid;

	};

	struct LobbyServerNoResPacket
	{
		SessionId_t sid;

		int8_t lobbyServerNo;
	};

	struct FailPacket
	{
		SessionId_t sid;
		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__FRONT_CENTER_PACKET_H__
