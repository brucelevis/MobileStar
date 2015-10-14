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
    
    
    //////////////////////////////////////// front recv
    
    
//    void frontHandleMoveChannelReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//    void frontHandleGetRoomListReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    
    //////////////////////////////////////// lobby recv
    
    void lobbyReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void lobbySessionIn(ConnectInfo* connectInfo);
    void lobbySessionOut(ConnectInfo* connectInfo);
    
    void lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void lobbyHandleEnterClientReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void lobbyHandleEnterClientOk(ConnectInfo* connectInfo, const char* body, int bodySize);
    void lobbyHandleEnterClientOut(ConnectInfo* connectInfo, const char* body, int bodySize);


    ////////////////////////////////////////
    
    
    void clientReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void clientSessionIn(ConnectInfo* connectInfo);
    void clientSessionOut(ConnectInfo* connectInfo);
    
    void clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleMoveLocationReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void clientHandleSendChattingReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    
    
    //////////////to send client
    
    
public:
    char sendBuffer[5000];
    
};

#endif // __I_O_MANAGER_H__