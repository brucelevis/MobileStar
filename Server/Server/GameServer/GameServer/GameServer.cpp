

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "GameDefine.h"
#include "GameServer.h"
#include "Network.h"
#include "Log.h"
#include "GameData.h"
#include "Room.h"
#include "User.h"
#include "ServerInfo.h"
#include "IOManager.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/GameLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MobileStar/Server/Server/Log/GameLog/";
#endif

GameServer* GameServer::m_instance = NULL;


bool GameServer::initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(LOG_PATH) == false)
    {
        return false;
    }
    
    gameData = new GameData();
    if (!gameData->Initialize())
    {
        ErrorLog("gameData Initialize");
        return false;
    }
    
    WorkerThread** workerThreadArray = new WorkerThread*[workerThreadCount];
    
    for(int i = 0; i < workerThreadCount; i++)
    {
        workerThreadArray[i] = new IOManager();
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



