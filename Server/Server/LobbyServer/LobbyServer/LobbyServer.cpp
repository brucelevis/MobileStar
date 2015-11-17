

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "LobbyDefine.h"
#include "LobbyServer.h"
#include "Network.h"
#include "Log.h"
#include "LobbyData.h"
#include "Channel.h"
#include "Room.h"
#include "User.h"
#include "IOManager.h"
#include "ServerInfo.h"
#include "QuickPlay.h"
#include "Clan.h"
#include "TimerList.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/LobbyLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MobileStar/Server/Server/Log/LobbyLog/";
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
        workerThreadArray[i] = new IOManager();
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
    
    clanMgr = new ClanManager();
    if(clanMgr->initialize() == false)
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



