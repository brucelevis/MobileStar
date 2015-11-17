
#include "ServerInfo.h"
#include "Log.h"

ServerInfoManager::ServerInfoManager()
{
    
}

ServerInfoManager::~ServerInfoManager()
{
    
}

bool ServerInfoManager::initialize()
{
    cacheServerInfo = NULL;
    chattingServerInfo = NULL;
    lobbyServerInfoList.clear();
    lobbyNoCreator = 1;
    return true;
}

bool ServerInfoManager::addCacheServer(ConnectInfo* connectInfo, char* lobbyIp, uint16_t lobbyPort)
{
    if(cacheServerInfo != NULL)
    {
        ErrorLog("cacheServerInfo already exist");
        delete cacheServerInfo;
    }
    
    cacheServerInfo = new CacheServerInfo();
    memcpy(cacheServerInfo->lobbyIp, lobbyIp, MAX_IP_ADDRESS_LEN);
    cacheServerInfo->lobbyPort = lobbyPort;
    
    cacheServerInfo->connectInfo = connectInfo;
    
    connectInfo->userData = (void*)cacheServerInfo;
    
    return true;
}

bool ServerInfoManager::addChattingServer(ConnectInfo* connectInfo, char * lobbyIp, uint16_t lobbyPort, char * gameIp, uint16_t gamePort, char * clientIp, uint16_t clientPort)
{
    if(chattingServerInfo != NULL)
    {
        ErrorLog("chattingServer already exist");
        delete chattingServerInfo;
    }
    
    chattingServerInfo = new ChattingServerInfo();
    memcpy(chattingServerInfo->lobbyIp, lobbyIp, MAX_IP_ADDRESS_LEN);
    chattingServerInfo->lobbyPort = lobbyPort;
    memcpy(chattingServerInfo->gameIp, gameIp, MAX_IP_ADDRESS_LEN);
    chattingServerInfo->gamePort = gamePort;
    memcpy(chattingServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    chattingServerInfo->clientPort = clientPort;
    
    chattingServerInfo->connectInfo = connectInfo;
    
    connectInfo->userData = (void*)chattingServerInfo;
    
    return true;
}

bool ServerInfoManager::addLobbyServer(ConnectInfo* connectInfo, char* gameIp, uint16_t gamePort, char* clientIp, uint16_t clientPort)
{
    LobbyServerInfo* lobbyServerInfo = new LobbyServerInfo();
    
    lobbyServerInfo->lobbyNo = lobbyNoCreator++;
    
    memcpy(lobbyServerInfo->gameIp, gameIp, MAX_IP_ADDRESS_LEN);
    lobbyServerInfo->gamePort = gamePort;
    memcpy(lobbyServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    lobbyServerInfo->clientPort = clientPort;
    
    lobbyServerInfo->connectInfo = connectInfo;
    connectInfo->userData = (void*)lobbyServerInfo;
    
    lobbyServerInfoList.push_back(lobbyServerInfo);
    
    return true;
}

LobbyServerInfo* ServerInfoManager::getLobbyServerInfo(int lobbyNo)
{
    for(int i = 0; i < lobbyServerInfoList.size(); i++)
    {
        if(lobbyServerInfoList[i]->lobbyNo == lobbyNo)
        {
            return lobbyServerInfoList[i];
        }
    }
    
    ErrorLog("lobby server no not exist lobbyNo");
    
    return NULL;
}