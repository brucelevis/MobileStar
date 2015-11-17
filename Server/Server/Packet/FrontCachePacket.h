#ifndef __FRONT_CACHE_PACKET_H__
#define __FRONT_CACHE_PACKET_H__

#include "BasicPacket.h"

class FrontCachePacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;

	enum EnumCommand // start number is at least 3.
	{
        FIRST_CONNECT_REQ = 10,
        FIRST_CONNECT_RES,
        FIRST_CONNECT_FAIL,

		CREATE_USER_REQ,
		CREATE_USER_RES,
		CREATE_USER_FAIL,

		LOGIN_USER_REQ,
		LOGIN_USER_RES,
		LOGIN_USER_FAIL,

		LOGOUT_USER_REQ,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
		EXIST_USER,
		EXIST_NICK_NAME,
		NOT_EXIST_USER,
		PASSWORD_INCORRECT,
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

	struct CreateUserReqPacket : public Packet
	{
        CreateUserReqPacket() { cmd = CREATE_USER_REQ; }

		SessionId_t sid;

		int16_t userIdLen;
		char userId[MAX_USER_ID_LEN];
		int8_t nickNameLen;
		char nickName[MAX_NICK_NAME_LEN];
	};

	struct CreateUserResPacket : public Packet
	{
        CreateUserResPacket() { cmd = CREATE_USER_RES; }

		SessionId_t sid;
        
        int64_t userNo;
	};

	struct LoginUserReqPacket : public Packet
	{
        LoginUserReqPacket() { cmd = LOGIN_USER_REQ; }

		SessionId_t sid;

        int16_t userIdLen;
        char userId[MAX_USER_ID_LEN];
	};

	struct LoginUserResPacket : public Packet
	{
        LoginUserResPacket() { cmd = LOGIN_USER_RES; }

		SessionId_t sid;

		int64_t userNo;
	};

	struct FailPacket : public Packet
	{
		SessionId_t sid;

		int32_t failReason;
	};

};

#pragma pack(pop)
#endif //__FRONT_CACHE_PACKET_H__
