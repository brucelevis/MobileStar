

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "FrontDefine.h"
#include "FrontServer.h"
#include "Network.h"
#include "Log.h"
#include "FrontData.h"
#include "ServerInfo.h"
#include "User.h"
#include "IOManager.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/FrontLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MobileStar/Server/Server/Log/FrontLog/";
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
        workerThreadArray[i] = new IOManager();
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



