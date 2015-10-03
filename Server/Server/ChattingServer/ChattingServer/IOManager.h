#ifndef __I_O_MANAGER_H__
#define __I_O_MANAGER_H__

#include <vector>
#include "WorkerThread.h"
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
    void connected(const ConnectInfo* connectInfo);
    void disconnected(const ConnectInfo* connectInfo);
    void receiveData(const ConnectInfo* connectInfo, const char* data, int dataSize);
    
    
    ////////////////////////////////////////
    void receiveClientData(const ConnectInfo* connectInfo, const char* data, int dataSize);
    void receiveMasterData(const ConnectInfo* connectInfo, const char* data, int dataSize);

    
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
};

#endif // __I_O_MANAGER_H__