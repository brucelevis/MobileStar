#ifndef __I_O_MANAGER_H__
#define __I_O_MANAGER_H__

#include <vector>
#include "WorkerThread.h"
#include "BasicPacket.h"
#include <map>

class IOManager : public WorkerThread
{
public:
    IOManager();
    ~IOManager();

    ////////////virtual method///////////////
    void connected(ConnectInfo* connectInfo);
    void disconnected(ConnectInfo* connectInfo);
    void receiveData(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    ////////////////////////////////////////
    void receiveClientData(const ConnectInfo* connectInfo, const char* data, int dataSize);
    void receiveMasterData(const ConnectInfo* connectInfo, const char* data, int dataSize);

    
    //////////////to send client
    
    
    
    
    
    
    ////////////////////////recv front
    void frontSessionIn(ConnectInfo* connectInfo);
    void frontSessionOut(ConnectInfo* connectInfo);
    void frontReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void frontHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void frontHandleLoginUserReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void frontHandleCreateUserReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void frontHandleLogoutClientReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    ////////////////////////recv lobby
    void lobbySessionIn(ConnectInfo* connectInfo);
    void lobbySessionOut(ConnectInfo* connectInfo);
    void lobbyReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleLoginUserReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    void lobbyHandleGetUserInfoReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleAddFriendReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleRemoveFriendReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleCreateClanReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleEnterClanReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleLeaveClanReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleChangeClanClassReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleDestroyClanReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    void lobbyHandleLogoutClientReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    ////////////////////////recv game
    void gameSessionIn(ConnectInfo* connectInfo);
    void gameSessionOut(ConnectInfo* connectInfo);
    void gameReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void gameHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    ////////////////////////recv chating
    void chattingSessionIn(ConnectInfo* connectInfo);
    void chattingSessionOut(ConnectInfo* connectInfo);
    void chattingReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void chattingHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    
    ////////////////////////recv client
    void clientSessionIn(ConnectInfo* connectInfo);
    void clientSessionOut(ConnectInfo* connectInfo);
    void clientReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleLoginReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleSignUpReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleEnterLobbyReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleEnterLobbyOk(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
public:
    
    char sendBuffer[5000];
    
    int createUserNo;
};

#endif // __I_O_MANAGER_H__