#ifndef __I_O_MANAGER_H__
#define __I_O_MANAGER_H__

#include <vector>
#include "WorkerThread.h"
#include "BasicPacket.h"
#include <map>

class FrontReceiveHandler;
class FrontSendHandler;
class LobbyReceiveHandler;
class LobbySendHandler;
class GameReceiveHandler;
class GameSendHandler;
class ChattingReceiveHandler;
class ChattingSendHandler;
class ClientReceiveHandler;
class ClientSendHandler;

class Room;
class IOManager : public WorkerThread
{
public:
    IOManager(FrontSendHandler* _frontSendHandler, LobbySendHandler* _lobbySendHandler, GameSendHandler* _gameSendHandler, ChattingSendHandler* _chattingSendHandler, ClientSendHandler* _clientSendHandler, FrontReceiveHandler* _frontReceiveHandler, LobbyReceiveHandler* _lobbyReceiveHandler, GameReceiveHandler* _gameReceiveHandler, ChattingReceiveHandler* _chattingReceiveHandler, ClientReceiveHandler* _clientReceiveHandler);
    ~IOManager();

    ////////////virtual method///////////////
    void connected(ConnectInfo* connectInfo);
    void disconnected(ConnectInfo* connectInfo);
    void receiveData(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    //////////////////////////////////////// front recv
    
    void frontReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void frontSessionIn(ConnectInfo* connectInfo);
    void frontSessionOut(ConnectInfo* connectInfo);
    
    void frontHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void frontHandleEnterClientReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
    void frontHandleEnterClientOk(const ConnectInfo* connectInfo, const char* body, int bodySize);
    void frontHandleEnterClientOut(const ConnectInfo* connectInfo, const char* body, int bodySize);

    
//    void frontHandleMoveChannelReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//    void frontHandleGetRoomListReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    
    
    //////////////////////////////////////// game recv
    
    void gameReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void gameSessionIn(ConnectInfo* connectInfo);
    void gameSessionOut(ConnectInfo* connectInfo);
    
    void gameHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void gameHandleEnterRoomRes(const ConnectInfo* connectInfo, const char* body, int bodySize);
    void gameHandleFinishGameReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
    void gameHandleMovingClientDisconnect(const ConnectInfo* connectInfo, const char* body, int bodySize);
    
    
    ////////////////////////////////////////
    ////////////////////////////////////////
    
    
    void clientReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void clientSessionIn(ConnectInfo* connectInfo);
    void clientSessionOut(ConnectInfo* connectInfo);
    
    void clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleGetChannelListReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
    void clientHandleMoveChannelReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
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
    FrontSendHandler* frontSendHandler;
    LobbySendHandler* lobbySendHandler;
    GameSendHandler* gameSendHandler;
    ChattingSendHandler* chattingSendHandler;
    ClientSendHandler* clientSendHandler;
    
    FrontReceiveHandler* frontReceiveHandler;
    LobbyReceiveHandler* lobbyReceiveHandler;
    GameReceiveHandler* gameReceiveHandler;
    ChattingReceiveHandler* chattingReceiveHandler;
    ClientReceiveHandler* clientReceiveHandler;
    
    
    char sendBuffer[5000];
    
    
    int testNameInt;
};

#endif // __I_O_MANAGER_H__