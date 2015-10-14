#ifndef __CHATTING_FRONT_PACKET_H__
#define __CHATTING_FRONT_PACKET_H__

#include "BasicPacket.h"

class ChattingFrontPacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 1;

	enum EnumCommand // start number is at least 3.
	{
        FIRST_CONNECT_REQ = 1,
        FIRST_CONNECT_RES,
        FIRST_CONNECT_FAIL,
        
		ENTER_LOBBY_REQ,
		ENTER_LOBBY_RES,
		ENTER_LOBBY_FAIL,
        ENTER_LOBBY_OK,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
		EXIST_EMAIL,
		EXIST_NICK_NAME,
		NOT_EXIST_EMAIL,
		PASSWORD_INCORRECT,
		ALREADY_LOGIN,
		NOT_LOGIN,
	};
    

	///////////////////////////////////

#pragma pack(push, 1)

	///////////////////////////////////


    struct FirstConnectReqPacket : public Packet
    {
        FirstConnectReqPacket() { cmd = FIRST_CONNECT_REQ; }
        
        char lobbyIp[MAX_IP_ADDRESS_LEN];
        int16_t lobbyPort;
        
        char clientIp[MAX_IP_ADDRESS_LEN];
        int16_t clientPort;
    };
    
    struct FirstConnectResPacket : public Packet
    {
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }

    };

    struct EnterLobbyReqPacket : public Packet
    {
        EnterLobbyReqPacket() { cmd = ENTER_LOBBY_REQ; }
        
        int8_t lobbyNo;
    };
    
    struct EnterLobbyResPacket : public Packet
    {
        EnterLobbyResPacket() { cmd = ENTER_LOBBY_RES; }
        
        char ip[MAX_IP_ADDRESS_LEN];
        uint16_t port;
    };

    struct EnterLobbyOkPacket : public Packet
    {
        EnterLobbyOkPacket() { cmd = ENTER_LOBBY_OK; }
        
    };

    struct FailPacket : public Packet
	{
		int32_t failReason;
	};

};
#pragma pack(pop)
#endif //__CLIENT_FRONT_PACKET_H__
