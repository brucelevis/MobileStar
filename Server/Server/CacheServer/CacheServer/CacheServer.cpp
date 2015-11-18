

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "CacheDefine.h"
#include "CacheServer.h"
#include "Network.h"
#include "Log.h"
#include "CacheData.h"
#include "ServerInfo.h"
#include "User.h"
#include "IOManager.h"
#include "Clan.h"
#include "GameDBChannel.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/CacheLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MobileStar/Server/Server/Log/CacheLog/";
#endif

CacheServer* CacheServer::m_instance = NULL;


bool CacheServer::initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(LOG_PATH) == false)
    {
        return false;
    }
    
    cacheData = new CacheData();
    if (!cacheData->Initialize())
    {
        ErrorLog("cacheData Initialize");
        return false;
    }
    
    WorkerThread** workerThreadArray = new WorkerThread*[workerThreadCount];
    
    for(int i = 0; i < workerThreadCount; i++)
    {
        workerThreadArray[i] = new IOManager();
    }
    
    network = new Network();
    if (network->Initialize(cacheData->networkInfoList, cacheData->networkInfoCount, workerThreadCount, workerThreadArray) == false)
    {
        ErrorLog("Network error");
        return false;
    }
    
    clanMgr = new ClanManager();
    if(!clanMgr->initialize())
    {
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
    
    gameDBChannel = new GameDBChannel();
    if(!gameDBChannel->Initialize("localhost", "root", "1234", "WorldWarDB", 3306))
    {
        return false;
    }
    
    if(gameDBChannel->InitUserInMemory(clanMgr, userMgr) == false)
    {
        return false;
    }

	return true;
}

void CacheServer::run()
{
    network->ProcessEvent();
}



