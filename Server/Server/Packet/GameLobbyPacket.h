#ifndef __GAME_LOBBY_PACKET_H__
#define __GAME_LOBBY_PACKET_H__

#include "BasicPacket.h"

class GameLobbyPacket
{
public:
    ////////////Packet Command//////////////
    
    static const int32_t PACKET_VERSION = 7;
    
    enum EnumCommand // start number is at least 3.
    {
        FIRST_CONNECT_REQ = 10,
        FIRST_CONNECT_RES,
        FIRST_CONNECT_FAIL,
        
        ENTER_ROOM_REQ,
        ENTER_ROOM_RES,
        ENTER_ROOM_FAIL,
        
        START_GAME_YES,
        START_GAME_NO,
        
        FINISH_GAME_REQ,
        FINISH_GAME_RES,
        FINISH_GAME_FAIL,
        MOVING_CLIENT_DISCONNCET,
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
        
        char clientIp[MAX_IP_ADDRESS_LEN];
        int16_t clientPort;
    };
    
    struct FirstConnectResPacket : public Packet
    {
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }
        
        char clientIp[MAX_IP_ADDRESS_LEN];
        int16_t clientPort;
    };
    
    struct EnterRoomReqPacket : public Packet
    {
        EnterRoomReqPacket() { cmd = ENTER_ROOM_REQ; }
        
        int16_t roomNo;
        int8_t userCount;
        UserInfoWithSessionId userInfoWithSessionId[MAX_ROOM_SLOT_COUNT];
    };
    
    struct EnterRoomResPacket : public Packet
    {
        EnterRoomResPacket() { cmd = ENTER_ROOM_RES; }
        
        int16_t roomNo;
    };
    
    
    struct StartGameYesPacket : public Packet
    {
        StartGameYesPacket() { cmd = START_GAME_YES; }
        
        int16_t roomNo;
    };
    
    struct StartGameNoPacket : public Packet
    {
        StartGameNoPacket() { cmd = START_GAME_NO; }
        
        int16_t roomNo;
    };
    
    
    struct FinishGameReqPacket : public Packet
    {
        FinishGameReqPacket() { cmd = FINISH_GAME_REQ; }
        
        int16_t roomNo;
    };
    
    struct FinishGameResPacket : public Packet
    {
        FinishGameResPacket() { cmd = FINISH_GAME_RES; }
        
        int16_t roomNo;
    };
    
    struct MovingClientDisconnectPacket : public Packet
    {
        MovingClientDisconnectPacket() { cmd = MOVING_CLIENT_DISCONNCET; }
        
        int16_t userNo;
    };
    
    
    
    
    struct FailPacket
    {
        int32_t failReason;
    };
};

#pragma pack(pop)
#endif //__GAME_LOBBY_PACKET_H__
