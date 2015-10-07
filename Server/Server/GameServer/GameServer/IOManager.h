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
class ClientReceiveHandler;
class ClientSendHandler;

class Room;

class IOManager : public WorkerThread
{
public:
    IOManager(FrontSendHandler* _frontSendHandler, LobbySendHandler* _lobbySendHandler, GameSendHandler* _gameSendHandler, ClientSendHandler* _clientSendHandler, FrontReceiveHandler* _frontReceiveHandler, LobbyReceiveHandler* _lobbyReceiveHandler, GameReceiveHandler* _gameReceiveHandler, ClientReceiveHandler* _clientReceiveHandler);
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
    
    
    //////////////////////////////////////// lobby recv
    
    void lobbyReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void lobbySessionIn(ConnectInfo* connectInfo);
    void lobbySessionOut(ConnectInfo* connectInfo);
    
    void lobbyHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void lobbyHandleEnterRoomReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    void lobbyHandleStartGameYes(ConnectInfo* connectInfo, const char* body, int bodySize);
    void lobbyHandleStartGameNo(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    void lobbyHandleFinishGameRes(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    ////////////////////////////////////////
    ////////////////////////////////////////
    
    
    void clientReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
    void clientSessionIn(ConnectInfo* connectInfo);
    void clientSessionOut(ConnectInfo* connectInfo);
    
    void clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize);
    void clientHandleClientNotify(ConnectInfo* connectInfo, const char* body, int bodySize);
    void clientHandleFinishGameReq(ConnectInfo* connectInfo, const char* body, int bodySize);
    void clientHandleMoveLobbyOk(ConnectInfo* connectInfo, const char* body, int bodySize);
    
    
    
    //////////////to send client
    
    
public:
    FrontSendHandler* frontSendHandler;
    LobbySendHandler* lobbySendHandler;
    GameSendHandler* gameSendHandler;
    ClientSendHandler* clientSendHandler;
    
    FrontReceiveHandler* frontReceiveHandler;
    LobbyReceiveHandler* lobbyReceiveHandler;
    GameReceiveHandler* gameReceiveHandler;
    ClientReceiveHandler* clientReceiveHandler;
};

#endif // __I_O_MANAGER_H__