#ifndef __LOBBY_FRONT_PACKET_H__
#define __LOBBY_FRONT_PACKET_H__

#include "BasicPacket.h"

class LobbyFrontPacket
{
public:
	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;

	enum EnumCommand // start number is at least 3.
	{
		FIRST_CONNECT_REQ = 10,
		FIRST_CONNECT_RES,
		FIRST_CONNECT_FAIL,

		ENTER_CLIENT_REQ,
		ENTER_CLIENT_RES,
		ENTER_CLIENT_FAIL,
        ENTER_CLIENT_OK,
        ENTER_CLIENT_OUT,
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
        
        char gameIp[MAX_IP_ADDRESS_LEN];
        int16_t gamePort;
        
		char clientIp[MAX_IP_ADDRESS_LEN];
		int16_t clientPort;
	};

	struct FirstConnectResPacket : public Packet
	{
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }
        
        char chattingIp[MAX_IP_ADDRESS_LEN];
        int16_t chattingPort;
	};

	struct EnterClientReqPacket : public Packet
	{
        EnterClientReqPacket() { cmd = ENTER_CLIENT_REQ; }
        
        SessionId_t sid;

		int64_t userNo;
	};

	struct EnterClientResPacket : public Packet
	{
        EnterClientResPacket() { cmd = ENTER_CLIENT_RES; }

        int64_t userNo;
    };

    struct EnterClientOkPacket : public Packet
    {
        EnterClientOkPacket() { cmd = ENTER_CLIENT_OK; }
        
        int64_t userNo;
    };
    
    struct EnterClientOutPacket : public Packet
    {
        EnterClientOutPacket() { cmd = ENTER_CLIENT_OUT; }
        
        int64_t userNo;
    };
    
    
	struct FailPacket
	{
		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__FRONT_LOBBY_PACKET_H__
