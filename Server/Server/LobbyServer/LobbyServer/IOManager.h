#ifndef __I_O_MANAGER_H__
#define __I_O_MANAGER_H__

#include <vector>
#include "WorkerThread.h"
#include "BasicPacket.h"
#include <map>

class Room;

class IOManager : public WorkerThread
{
public:
    IOManager();
    ~IOManager();

    ////////////virtual method///////////////
    void connected(ConnectInfo* connectInfo);
    void disconnected(ConnectInfo* connectInfo);
    void receiveData(ConnectInfo* connectInfo, const char* data, int dataSize);

    
    //////////////////////////////////////// cache recv
    
    void cacheReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void cacheSessionIn(ConnectInfo* connectInfo);
    void cacheSessionOut(ConnectInfo* connectInfo);
    
    void cacheHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void cacheHandleLoginUserRes(ConnectInfo* connectInfo, const char* body, int bodySize);
//    void frontHandleEnterClientOk(ConnectInfo* connectInfo, const char* body, int bodySize);
//    void frontHandleEnterClientOut(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    //////////////////////////////////////// front recv
    
    void frontReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void frontSessionIn(ConnectInfo* connectInfo);
    void frontSessionOut(ConnectInfo* connectInfo);
    
    void frontHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void frontHandleEnterClientReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void frontHandleEnterClientOk(ConnectInfo* connectInfo, const char* body, int bodySize);
    void frontHandleEnterClientOut(ConnectInfo* connectInfo, const char* body, int bodySize);

    
//    void frontHandleMoveChannelReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//    void frontHandleGetRoomListReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    
    //////////////////////////////////////// lobby recv
    
    void lobbyReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void lobbySessionIn(ConnectInfo* connectInfo);
    void lobbySessionOut(ConnectInfo* connectInfo);
    
    void lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    
    
    
    
    //////////////////////////////////////// game recv
    
    void gameReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void gameSessionIn(ConnectInfo* connectInfo);
    void gameSessionOut(ConnectInfo* connectInfo);
    
    void gameHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void gameHandleEnterRoomRes(ConnectInfo* connectInfo, const char* body, int bodySize);
    void gameHandleFinishGameReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void gameHandleMovingClientDisconnect(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    
    //////////////////////////////////////// chatting recv
    
    void chattingReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void chattingSessionIn(ConnectInfo* connectInfo);
    void chattingSessionOut(ConnectInfo* connectInfo);
    
    void chattingHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void chattingHandleEnterClientRes(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    
    
    ////////////////////////////////////////
    
    
    void clientReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void clientSessionIn(ConnectInfo* connectInfo);
    void clientSessionOut(ConnectInfo* connectInfo);
    
    void clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleGetChannelListReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void clientHandleMoveChannelReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void clientHandleGetUserListReq(ConnectInfo* connectInfo, const char* body, int bodySize);//
    void clientHandleGetUserInfoReq(ConnectInfo* connectInfo, const char* body, int bodySize);//
    void clientHandleRequestGameReq(ConnectInfo* connectInfo, const char* body, int bodySize);//
    void clientHandleRequestGameCancelReq(ConnectInfo* connectInfo, const char* body, int bodySize);//
    void clientHandleResponceGameYesReq(ConnectInfo* connectInfo, const char* body, int bodySize);//
    void clientHandleResponceGameNoReq(ConnectInfo* connectInfo, const char* body, int bodySize);//
    
    void clientHandleGetRoomListReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleCreateRoomReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleEnterRoomReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleReadyReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleUnReadyReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleChangeTribeReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleLeaveRoomReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleStartGameOk(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleQuickPlayReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleQuickPlayOk(ConnectInfo* connectInfo, const char* body, const int bodySize);
    
    
    void clientSendLeaveRoomNotify(Room* room, int8_t leaveIndex);

    
    //////////////to send client
    
    
public:
    char sendBuffer[5000];
    
};

#endif // __I_O_MANAGER_H__