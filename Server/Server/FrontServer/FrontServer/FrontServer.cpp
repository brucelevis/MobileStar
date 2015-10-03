

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "FrontDefine.h"
#include "FrontServer.h"
#include "Network.h"
#include "Log.h"
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
#include "FrontData.h"
#include "ServerInfo.h"
#include "User.h"
#include "IOManager.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/FrontLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MoblieStar/Server/Server/Log/FrontLog/";
#endif

FrontServer* FrontServer::m_instance = NULL;


bool FrontServer::initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(LOG_PATH) == false)
    {
        return false;
    }
    
    frontData = new FrontData();
    if (!frontData->Initialize())
    {
        ErrorLog("lobbyData Initialize");
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
        
        
        ChattingReceiveHandler* chattingReceiveHandler = new ChattingReceiveHandler();
        if (!chattingReceiveHandler->initialize())
        {
            ErrorLog("chattingReceiveHandler Initialize");
            return false;
        }
        ChattingSendHandler* chattingSendHandler = new ChattingSendHandler();
        if (!chattingSendHandler->initialize())
        {
            ErrorLog("chattingSendHandler Initialize");
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
        
        workerThreadArray[i] = new IOManager(frontSendHandler, lobbySendHandler, gameSendHandler, chattingSendHandler, clientSendHandler, frontReceiveHandler, lobbyReceiveHandler, gameReceiveHandler, chattingReceiveHandler, clientReceiveHandler);
    }
    
    network = new Network();
    if (network->Initialize(frontData->networkInfoList, frontData->networkInfoCount, workerThreadCount, workerThreadArray) == false)
    {
        ErrorLog("Network error");
        return false;
    }

	userMgr = new UserManager();
	if (!userMgr->initialize())
	{
		return false;
	}

	serverInfoMgr = new ServerInfoManager();
	if (!serverInfoMgr->initialize())
	{
		return false;
	}

	return true;
}

void FrontServer::run()
{
    network->ProcessEvent();
}



