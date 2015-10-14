#ifndef __CLIENT_CHATTING_PACKET_H__
#define __CLIENT_CHATTING_PACKET_H__

#include "BasicPacket.h"

class ClientChattingPacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 1;

	enum EnumCommand // start number is at least 3.
	{
        FIRST_CONNECT_REQ = 1,
        FIRST_CONNECT_RES,
        FIRST_CONNECT_FAIL,
        FIRST_CONNECT_OK,
        FIRST_CONNECT_OUT,
        
		MOVE_LOCATION_REQ,
		MOVE_LOCATION_RES,
		MOVE_LOCATION_FAIL,
        
        SEND_CHATTING_REQ,
        SEND_CHATTING_RES,
        SEND_CHATTING_NOTIFY,
        SEND_CHATTING_FAIL,
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
        SessionId_t sessionId;
    };
    
    struct FirstConnectResPacket : public Packet
    {
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }

    };
    
    struct FirstConnectOkPacket : public Packet
    {
        FirstConnectOkPacket() { cmd = FIRST_CONNECT_OK; }
        
    };
    
    struct FirstConnectOutPacket : public Packet
    {
        FirstConnectOutPacket() { cmd = FIRST_CONNECT_OUT; }
        
    };

    struct MoveLocationReqPacket : public Packet
    {
        MoveLocationReqPacket() { cmd = MOVE_LOCATION_REQ; }
        
        int32_t loNo;
    };
    
    struct MoveLocationResPacket : public Packet
    {
        MoveLocationResPacket() { cmd = MOVE_LOCATION_RES; }
        
    };
    
    struct SendChattingReqPacket : public Packet
    {
        SendChattingReqPacket() { cmd = SEND_CHATTING_REQ; }
        
        uint8_t chattingLen;
        char chatting[MAX_CHATTING_LEN];
    };

    
    struct SendChattingResPacket : public Packet
    {
        SendChattingResPacket() { cmd = SEND_CHATTING_RES; }
        
    };

    
    struct SendChattingNotifyPacket : public Packet
    {
        SendChattingNotifyPacket() { cmd = SEND_CHATTING_NOTIFY; }
        
        int8_t nickNameLen;
        char nickName[MAX_NICK_NAME_LEN];
        
        uint8_t chattingLen;
        char chatting[MAX_CHATTING_LEN];
    };

    struct FailPacket : public Packet
	{
		int32_t failReason;
	};

};
#pragma pack(pop)
#endif //__CLIENT_CHATTING_PACKET_H__
