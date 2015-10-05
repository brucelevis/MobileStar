

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "LobbyDefine.h"
#include "LobbyServer.h"
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
#include "LobbyData.h"
#include "Channel.h"
#include "Room.h"
#include "User.h"
#include "IOManager.h"
#include "ServerInfo.h"
#include "QuickPlay.h"
#include "TimerList.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/LobbyLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MoblieStar/Server/Server/Log/LobbyLog/";
#endif

LobbyServer* LobbyServer::m_instance = NULL;


bool LobbyServer::initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(LOG_PATH) == false)
    {
        return false;
    }
    
    lobbyData = new LobbyData();
    if (!lobbyData->Initialize())
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
    if (network->Initialize(lobbyData->networkInfoList, lobbyData->networkInfoCount, workerThreadCount, workerThreadArray) == false)
    {
        ErrorLog("Network error");
        return false;
    }

	userMgr = new UserManager();
	if (!userMgr->initialize())
	{
		return false;
	}

    ChannelInfo channelInfo[3];
    for(int i = 1; i < 4; i++)
    {
        channelInfo[i-1].channelNo = i;
        channelInfo[i-1].channelNameLen = 8;
        sprintf(channelInfo[i-1].channelName, "channel%d", i);
    }
    
	channelMgr = new ChannelManager();
	if (!channelMgr->initialize(3, channelInfo))
	{
		return false;
	}
    
    roomMgr = new RoomManager();
    if(roomMgr->initialize() == false)
    {
        return false;
    }
    
    serverInfoMgr = new ServerInfoManager();
    if(serverInfoMgr->initialize() == false)
    {
        return false;
    }
    
    quickPlayMgr = new QuickPlayManager();
    if(quickPlayMgr->initialize() == false)
    {
        return false;
    }

	return true;
}

void LobbyServer::run()
{
    MatchUserTimer* mut = new MatchUserTimer(1, 1000);
    if(network->AddTimer(mut) == false)
    {
        return ;
    }
    
    UserListInChannelTimer* ulict = new UserListInChannelTimer(2, 1000);
    if(network->AddTimer(ulict) == false)
    {
        return ;
    }
    
    
    
    network->ProcessEvent();
}



