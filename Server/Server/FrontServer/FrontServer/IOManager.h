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


class IOManager : public WorkerThread
{
public:
    IOManager(FrontSendHandler* _frontSendHandler, LobbySendHandler* _lobbySendHandler, GameSendHandler* _gameSendHandler, ChattingSendHandler* _chattingSendHandler, ClientSendHandler* _clientSendHandler, FrontReceiveHandler* _frontReceiveHandler, LobbyReceiveHandler* _lobbyReceiveHandler, GameReceiveHandler* _gameReceiveHandler, ChattingReceiveHandler* _chattingReceiveHandler, ClientReceiveHandler* _clientReceiveHandler);
    ~IOManager();

    ////////////virtual method///////////////
    void connected(ConnectInfo* connectInfo);
    void disconnected(ConnectInfo* connectInfo);
    void receiveData(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    ////////////////////////////////////////
    void receiveClientData(const ConnectInfo* connectInfo, const char* data, int dataSize);
    void receiveMasterData(const ConnectInfo* connectInfo, const char* data, int dataSize);

    
    //////////////to send client
    
    
    
    
    
    
    ////////////////////////recv login
    void loginSessionIn(ConnectInfo* connectInfo);
    void loginSessionOut(ConnectInfo* connectInfo);
    void loginReceiveData(ConnectInfo* connectInfo, const char* data, int dataSize);
    
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
    void chatingSessionIn(ConnectInfo* connectInfo);
    void chatingSessionOut(ConnectInfo* connectInfo);
    void chatingReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void chatingHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    
    ////////////////////////recv client
    void clientSessionIn(ConnectInfo* connectInfo);
    void clientSessionOut(ConnectInfo* connectInfo);
    void clientReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize);
    
    void clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleEnterLobbyReq(ConnectInfo* connectInfo, const char* data, int dataSize);
    void clientHandleEnterLobbyOk(ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
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
    
    
    
    int createUserNo;
};

#endif // __I_O_MANAGER_H__