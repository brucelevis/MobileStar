#ifndef __GAME_FRONT_PACKET_H__
#define __GAME_FRONT_PACKET_H__

#include "BasicPacket.h"

class GameFrontPacket
{
public:
	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;

	enum EnumCommand // start number is at least 3.
	{
		FIRST_CONNECT_REQ = 10,
		FIRST_CONNECT_RES,
		FIRST_CONNECT_FAIL,
	};

	enum FailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
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
        
        char lobbyIp[MAX_IP_ADDRESS_LEN];
        int16_t lobbyPort;
	};

	struct FailPacket
	{
		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__GAME_FRONT_PACKET_H__
