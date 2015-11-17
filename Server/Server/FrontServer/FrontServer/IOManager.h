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
    
    
    
    
    
    
    ////////////////////////recv cache
    void cacheSessionIn(ConnectInfo* connectInfo);
    void cacheSessionOut(ConnectInfo* connectInfo);
    void cacheReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void cacheHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void cacheHandleLoginUserRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    void cacheHandleLoginUserFail(ConnectInfo* connectInfo, const char* data, int dataSize);
    void cacheHandleCreateUserRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    ////////////////////////recv lobby
    void lobbySessionIn(ConnectInfo* connectInfo);
    void lobbySessionOut(ConnectInfo* connectInfo);
    void lobbyReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void lobbyHandleEnterClientRes(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
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
    void clientHandleCreateAccountReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleEnterLobbyReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleEnterLobbyOk(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
public:
    
    char sendBuffer[5000];
    
    int createUserNo;
};

#endif // __I_O_MANAGER_H__