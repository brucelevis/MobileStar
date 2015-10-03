#include "IOManager.h"
#include "Log.h"
#include "Network.h"

#include "ChattingDefine.h"
#include "ChattingServer.h"

#include "FrontReceiveHandler.h"
#include "FrontSendHandler.h"
#include "LobbyReceiveHandler.h"
#include "LobbySendHandler.h"
#include "GameReceiveHandler.h"
#include "GameSendHandler.h"
#include "ChattingReceiveHandler.h"
#include "ChattingSendHandler.h"
#include "ClientReceiveHandler.h"
#include "ClientSendHandler.h"

IOManager::IOManager(FrontSendHandler* _frontSendHandler, LobbySendHandler* _lobbySendHandler, GameSendHandler* _gameSendHandler, ChattingSendHandler* _chattingSendHandler, ClientSendHandler* _clientSendHandler, FrontReceiveHandler* _frontReceiveHandler, LobbyReceiveHandler* _lobbyReceiveHandler, GameReceiveHandler* _gameReceiveHandler, ChattingReceiveHandler* _chattingReceiveHandler, ClientReceiveHandler* _clientReceiveHandler)
{
    frontReceiveHandler = _frontReceiveHandler;
    frontSendHandler = _frontSendHandler;
    lobbyReceiveHandler = _lobbyReceiveHandler;
    lobbySendHandler = _lobbySendHandler;
    gameReceiveHandler = _gameReceiveHandler;
    gameSendHandler = _gameSendHandler;
    chattingReceiveHandler = _chattingReceiveHandler;
    chattingSendHandler = _chattingSendHandler;
    clientReceiveHandler = _clientReceiveHandler;
    clientSendHandler = _clientSendHandler;
}


IOManager::~IOManager()
{
    delete frontReceiveHandler;
    delete frontSendHandler;
    delete lobbyReceiveHandler;
    delete lobbySendHandler;
    delete gameReceiveHandler;
    delete gameSendHandler;
    delete clientReceiveHandler;
    delete clientSendHandler;
}

void IOManager::connected(const ConnectInfo* connectInfo)
{
    DebugLog("connected");
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveHandler->sessionIn(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveHandler->sessionIn(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveHandler->sessionIn(connectInfo);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveHandler->sessionIn(connectInfo);
            break;
        case SERVER_MODULE_CLIENT:
            clientReceiveHandler->sessionIn(connectInfo);
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            break;
    }
    
#if THREAD_TYPE == SINGLE_THREAD
#else
    ChattingServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
#endif
}


void IOManager::disconnected(const ConnectInfo* connectInfo)
{
    DebugLog("disconnected");

    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveHandler->sessionOut(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveHandler->sessionOut(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveHandler->sessionOut(connectInfo);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveHandler->sessionOut(connectInfo);
            break;
        case SERVER_MODULE_CLIENT:
            clientReceiveHandler->sessionOut(connectInfo);
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            break;
    }
    
#if THREAD_TYPE == SINGLE_THREAD
#else
    ChattingServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
#endif
}


void IOManager::receiveData(const ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DebugLog("reciveData");
    
    if (dataSize < sizeof(commandType_t))
    {
        ErrorLog("dataSize too smail - %d", dataSize);
        //sessionOut
        return ;
    }
    
    const char* pData = data;
    
    commandType_t cmd;
    memcpy(&cmd, data, sizeof(commandType_t));
    pData += sizeof(commandType_t);
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(commandType_t));
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(commandType_t));
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(commandType_t));
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(commandType_t));
            break;
        case SERVER_MODULE_CLIENT:
            clientReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(commandType_t));
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            //sessionOut
            break;
    }
}




