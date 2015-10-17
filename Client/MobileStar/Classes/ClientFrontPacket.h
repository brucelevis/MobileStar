#ifndef __CLIENT_FRONT_PACKET_H__
#define __CLIENT_FRONT_PACKET_H__

#include "BasicPacket.h"

class ClientFrontPacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 1;

	enum EnumCommand // start number is at least 3.
	{
        FIRST_CONNECT_REQ = 1,
        FIRST_CONNECT_RES,
        FIRST_CONNECT_FAIL,
        
        LOGIN_REQ,
        LOGIN_RES,
        LOGIN_FAIL,
        
        SIGN_UP_REQ,
        SIGN_UP_RES,
        SIGN_UP_FAIL,
        
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
        int32_t packetVersion;
    };
    
    struct FirstConnectResPacket : public Packet
    {
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }
        SessionId_t sessionId;
    };
    
    struct LoginReqPacket : public Packet
    {
        LoginReqPacket() { cmd = LOGIN_REQ; }
        int8_t nickNameLen;
        char nickName[MAX_NICK_NAME_LEN];
        int8_t passwordLen;
        char password[MAX_PASSWORD_LEN];
    };
    
    struct LoginResPacket : public Packet
    {
        LoginResPacket() { cmd = LOGIN_RES; }
        UserInfo userInfo;
    };
    
    struct SignUpReqPacket : public Packet
    {
        SignUpReqPacket() { cmd = SIGN_UP_REQ; }
        int8_t nickNameLen;
        char nickName[MAX_NICK_NAME_LEN];
        int8_t passwordLen;
        char password[MAX_PASSWORD_LEN];
        int8_t emailLen;
        char email[MAX_EMAIL_LEN];
    };
    
    struct SignUpResPacket : public Packet
    {
        SignUpResPacket() { cmd = SIGN_UP_RES; }
        
    };
    

    struct EnterLobbyReqPacket : public Packet
    {
        EnterLobbyReqPacket() { cmd = ENTER_LOBBY_REQ; }
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
