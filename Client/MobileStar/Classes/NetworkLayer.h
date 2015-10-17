#ifndef __NETWORK_LAYER_H__
#define __NETWORK_LAYER_H__

#include "cocos2d.h"
#include "Network.h"

static const char* IP_ADDRESS = "1.234.65.217";
static const unsigned short PORT = 10400;

enum SERVER_MODULE
{
    SERVER_MODULE_FRONT_SERVER = 0,
    SERVER_MODULE_LOBBY_SERVER,
    SERVER_MODULE_GAME_SERVER,
    SERVER_MODULE_CHATTING_SERVER,
    
};

class NetworkHandler : public ReceiveHandler
{
public:
    
    bool initialize();
    virtual void Receive(ConnectInfo* connectInfo, const char* data, int dataSize);

    //////////////////////////////////HANDLE
    
    
    ////front
    
    void frontHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void frontHandleLoginRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void frontHandleSignUpRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void frontHandleEnterLobbyRes(ConnectInfo* connectInfo, const char* data, int dataSize);

    ////lobby
    void lobbyHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleChannelInfoNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleFirstConnectOk(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleFirstConnectOut(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleGetChannelListRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleMoveChannelRes(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    
    void lobbyHandleGetUserInfoRes(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    void lobbyHandleEnterUserInChannelNotify(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    void lobbyHandleLeaveUserInChannelNotify(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    void lobbyHandleRequestGameRes(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    void lobbyHandleRequestGameNotify(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    void lobbyHandleRequestGameCancelRes(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    void lobbyHandleResponseGameNoNotify(ConnectInfo* connectInfo, const char* pData, int pDataSize);
    
    
    void lobbyHandleGetRoomListRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleCreateRoomRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleEnterRoomRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleEnterRoomNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleReadyRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleReadyNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleUnReadyRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleUnReadyNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleChangeTribeRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleChangeTribeNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleLeaveRoomRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleLeaveRoomNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleStartGameNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleQuickPlayRes(ConnectInfo* connectInfo, const char* data, int dataSize);

    ////game
    
    void gameHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void gameHandleStartGameNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void gameHandleClientNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    void gameHandleFinishGameRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    ////chatting
    
    void chattingHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void chattingHandleMoveLocationRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void chattingHandleSendChattingNotify(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    ///////////////////// SEND
    
    
    void frontSendFirstConnectReq();
    void frontSendLoginReq(const char* nickName, int8_t nickNameLen, const char* password, int8_t passwordLen);
    void frontSendSignUpReq(const char* nickName, int8_t nickNameLen, const char* password, int8_t passwordLen, const char* email, int8_t emailLen);
    void frontSendEnterLobbyReq();
    void frontSendEnterLobbyOk();
    
    
    void lobbySendFirstConnectReq();
    void lobbySendGetChannelListReq();
    void lobbySendMoveChannelReq(int channelNo);
    
    void lobbySendGetUserInfoReq(int64_t userNo);
    void lobbySendRequestGameReq(int64_t userNo);
    void lobbySendRequestGameCancelReq();
    void lobbySendResponseGameYesReq(int64_t userNo);
    void lobbySendResponseGameNoReq(int64_t userNo);
    

    
    void lobbySendGetRoomListReq();
    void lobbySendCreateRoomReq(const char* roomName, int roomNameLen, int mapType);
    void lobbySendEnterRoomReq(int roomNo);
    void lobbySendReadyReq();
    void lobbySendUnReadyReq();
    void lobbySendChangeTribeReq(int tribe);
    void lobbySendLeaveRoomReq();
    void lobbySendQuickPlayReq(int tribe);
    
    
    void gameSendFirstConnectReq();
    void gameSendClientNotify(int bufferLen, const char* clientSendBuffer);
    void gameSendFinishGameReq();
    void gameSendMoveLobbyOk();
    
    void chattingSendFirstConnectReq();
    void chattingSendMoveLocationReq(int32_t loNo);
    void chattingSendSendChattingReq(const char* chatting, uint8_t chattingLen);
    
    
public:
    
    Network* network;

    char sendBuffer[5000];
};

class NetworkLayer : public cocos2d::Layer
{
public:
    virtual bool init();  
	void update(float dt);
	void AddThisToScene(cocos2d::Scene* scene);

    CREATE_FUNC(NetworkLayer);

    NetworkHandler* handler;
};

#endif
