

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "GameDefine.h"
#include "GameServer.h"
#include "Network.h"
#include "Log.h"
#include "FrontReceiveHandler.h"
#include "FrontSendHandler.h"
#include "LobbyReceiveHandler.h"
#include "LobbySendHandler.h"
#include "GameReceiveHandler.h"
#include "GameSendHandler.h"
#include "ClientReceiveHandler.h"
#include "ClientSendHandler.h"
#include "GameData.h"
#include "Room.h"
#include "User.h"
#include "ServerInfo.h"
#include "IOManager.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/GameLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MoblieStar/Server/Server/Log/GameLog/";
#endif

GameServer* GameServer::m_instance = NULL;


bool GameServer::initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(LOG_PATH) == false)
    {
        return false;
    }
    
    
//    int networkInfoCount = xmlData->serverInfoCount;
//    
//    NetworkInfo networkInfoList[networkInfoCount];
//    memset(networkInfoList, 0, sizeof(NetworkInfo) * networkInfoCount);
//    
//    for(int i = 0; i < networkInfoCount; i++)
//    {
//        networkInfoList[i].type = xmlData->serverInfoList[i].serverType;
//        networkInfoList[i].module = xmlData->serverInfoList[i].serverModule;
//        memcpy(networkInfoList[i].ip, xmlData->serverInfoList[i].ip, MAX_IP_LEN);
//        networkInfoList[i].port = xmlData->serverInfoList[i].port;
//        networkInfoList[i].sendPingInterval = 5000;
//        networkInfoList[i].disconnectPingCount = 3;
//        
//    }
    
    gameData = new GameData();
    if (!gameData->Initialize())
    {
        ErrorLog("gameData Initialize");
        return false;
    }
    
    WorkerThread** workerThreadArray = new WorkerThread*[workerThreadCount];
    
    for(int i = 0; i < workerThreadCount; i++)
    {
        FrontReceiveHandler* frontReceiveHandler = new FrontReceiveHandler();
        if (!frontReceiveHandler->initialize())
        {
            ErrorLog("_frontReceiveHandler Initialize");
            return false;
        }
        
        FrontSendHandler* frontSendHandler = new FrontSendHandler();
        if (!frontSendHandler->initialize())
        {
            ErrorLog("frontSendHandler Initialize");
            return false;
        }
        
        
        LobbyReceiveHandler* lobbyReceiveHandler = new LobbyReceiveHandler();
        if (!lobbyReceiveHandler->initialize())
        {
            ErrorLog("lobbyReceiveHandler Initialize");
            return false;
        }
        LobbySendHandler* lobbySendHandler = new LobbySendHandler();
        if (!lobbySendHandler->initialize())
        {
            ErrorLog("lobbySendHandler Initialize");
            return false;
        }
        
        
        GameReceiveHandler* gameReceiveHandler = new GameReceiveHandler();
        if (!gameReceiveHandler->initialize())
        {
            ErrorLog("_gameReceiveHandler Initialize");
            return false;
        }
        GameSendHandler* gameSendHandler = new GameSendHandler();
        if (!gameSendHandler->initialize())
        {
            ErrorLog("gameSendHandler Initialize");
            return false;
        }
        
        
        ClientReceiveHandler* clientReceiveHandler = new ClientReceiveHandler();
        if (!clientReceiveHandler->initialize())
        {
            ErrorLog("_clientReceiveHandler Initialize");
            return false;
        }
        ClientSendHandler* clientSendHandler = new ClientSendHandler();
        if (!clientSendHandler->initialize())
        {
            ErrorLog("clientSendHandler Initialize");
            return false;
        }
        
        workerThreadArray[i] = new IOManager(frontSendHandler, lobbySendHandler, gameSendHandler, clientSendHandler, frontReceiveHandler, lobbyReceiveHandler, gameReceiveHandler, clientReceiveHandler);
    }
    
    network = new Network();
    if (network->Initialize(gameData->networkInfoList, gameData->networkInfoCount, workerThreadCount, workerThreadArray) == false)
    {
        ErrorLog("Network error");
        return false;
    }

	userMgr = new UserManager();
	if (!userMgr->initialize())
	{
		return false;
	}

	roomMgr = new RoomManager();
	if (!roomMgr->initialize())
	{
		return false;
	}
    
    serverInfoMgr = new ServerInfoManager();
    if(!serverInfoMgr->initialize())
    {
        return false;
    }

	return true;
}

void GameServer::run()
{
    network->ProcessEvent();
}



