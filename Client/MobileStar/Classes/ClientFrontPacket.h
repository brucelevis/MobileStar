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
        
        CREATE_ACCOUNT_REQ,
        CREATE_ACCOUNT_RES,
        CREATE_ACCOUNT_FAIL,
        
        ENTER_LOBBY_REQ,
        ENTER_LOBBY_RES,
        ENTER_LOBBY_FAIL,
        ENTER_LOBBY_OK,
    };
    
    enum EnumFailReason
    {
        PACKET_VERSION_INCORRECT = 1,
        SERVER_ERROR,
        EXIST_LOGIN_TOKEN,
        EXIST_NICK_NAME,
        ALREADY_LOGIN,
        NOT_LOGIN,
    };
    
    enum LOGIN_RESULT
    {
        LOGIN_RESULT_SUCCESS = 1,
        LOGIN_RESULT_NOT_EXIST_USER_ID,
    };
    
    enum CREATE_ACCOUNT_RESULT
    {
        CREATE_ACCOUNT_RESULT_SUCCESS = 1,
        CREATE_ACCOUNT_RESULT_USER_ID_EXIST,
        CREATE_ACCOUNT_RESULT_NICKNAME_EXIST,
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
        
        int16_t userIdLen;
        char userId[MAX_USER_ID_LEN];
    };
    
    struct LoginResPacket : public Packet
    {
        LoginResPacket() { cmd = LOGIN_RES; }
        
        int8_t loginInfo;
    };
    
    struct CreateAccountReqPacket : public Packet
    {
        CreateAccountReqPacket() { cmd = CREATE_ACCOUNT_REQ; }
        
        int16_t userIdLen;
        char userId[MAX_USER_ID_LEN];
        int8_t nickNameLen;
        char nickName[MAX_NICK_NAME_LEN];
    };
    
    struct CreateAccountResPacket : public Packet
    {
        CreateAccountResPacket() { cmd = CREATE_ACCOUNT_RES; }
        
        int8_t isSuccess;
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
