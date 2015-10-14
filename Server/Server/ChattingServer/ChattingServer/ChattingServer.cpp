

//
// GameServer.cpp
//
// create by kimyc on 2014.4.7..
//

#include "ChattingDefine.h"
#include "ChattingServer.h"
#include "Network.h"
#include "Log.h"
#include "ChattingData.h"
#include "User.h"
#include "IOManager.h"
#include "LObject.h"

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_PATH = "../../Log/ChattingLog/";
#elif OS_PLATFORM == PLATFORM_MAC
const char* LOG_PATH = "/Users/kimyongchan/MobileStar/Server/Server/Log/ChattingLog/";
#endif

ChattingServer* ChattingServer::m_instance = NULL;


bool ChattingServer::initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(LOG_PATH) == false)
    {
        return false;
    }
    
    chattingData = new ChattingData();
    if (!chattingData->Initialize())
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
    if (network->Initialize(chattingData->networkInfoList, chattingData->networkInfoCount, workerThreadCount, workerThreadArray) == false)
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
    
    lObjectMgr = new LObjectManager();
    if (!lObjectMgr->initialize())
    {
        return false;
    }
    
    return true;
}

void ChattingServer::run()
{
    network->ProcessEvent();
}


