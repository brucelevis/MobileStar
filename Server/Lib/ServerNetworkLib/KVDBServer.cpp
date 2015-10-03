#include "KVDBServer.h"

#include "Network.h"
#include "XmlData.h"
#include "IOManager.h"

#include "Log.h"
#include <deque>


KVDBServer* KVDBServer::m_instance = NULL;

bool KVDBServer::Initialize(int workerThreadCount)
{
    log = new Log();
    if (log->Initialize(".") == false)
    {
        return false;
    }
    
    xmlData = new XmlData();
    if(xmlData->initialize() == false)
    {
        ErrorLog("xmlData error");
        return false;
    }

    int networkInfoCount = xmlData->serverInfoCount;
    
    NetworkInfo networkInfoList[networkInfoCount];
    memset(networkInfoList, 0, sizeof(NetworkInfo) * networkInfoCount);
    
    for(int i = 0; i < networkInfoCount; i++)
    {
        networkInfoList[i].type = xmlData->serverInfoList[i].serverType;
        networkInfoList[i].module = xmlData->serverInfoList[i].serverModule;
        memcpy(networkInfoList[i].ip, xmlData->serverInfoList[i].ip, MAX_IP_LEN);
        networkInfoList[i].port = xmlData->serverInfoList[i].port;
        networkInfoList[i].sendPingInterval = 5000;
        networkInfoList[i].disconnectPingCount = 3;
        
    }
    
    
    WorkerThread** workerThreadArray = new WorkerThread*[workerThreadCount];

    for(int i = 0; i < workerThreadCount; i++)
    {
        workerThreadArray[i] = new IOManager();
    }
    
    network = new Network();
    if (network->Initialize(networkInfoList, networkInfoCount, workerThreadCount, workerThreadArray) == false)
    {
        ErrorLog("Network error");
        return false;
    }
    
	return true;
}

void KVDBServer::Run()
{
	while (true)
	{
		network->ProcessEvent();
	}
}


