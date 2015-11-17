
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
    chattingServerInfo = NULL;
    createGameServerNo = 1;
    gameServerInfoList.clear();
    return true;
}

bool ServerInfoManager::addCacheServer(ConnectInfo* connectInfo)
{
    cacheServerInfo = new CacheServerInfo();
    
    cacheServerInfo->connectInfo = connectInfo;
    connectInfo->userData = (void*)cacheServerInfo;
    
    return true;
}

bool ServerInfoManager::addChattingServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort)
{
    chattingServerInfo = new ChattingServerInfo();
    
    memcpy(chattingServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    chattingServerInfo->clientPort = clientPort;
    
    chattingServerInfo->connectInfo = connectInfo;
    connectInfo->userData = (void*)chattingServerInfo;
    
    return true;
}

bool ServerInfoManager::removeChattingServer()
{
    delete chattingServerInfo;
    
    chattingServerInfo = NULL;
    
    return true;
}


bool ServerInfoManager::addGameServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort)
{
    GameServerInfo* gameServerInfo = new GameServerInfo();
    
    gameServerInfo->gameServerNo = createGameServerNo++;
    
    memcpy(gameServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    gameServerInfo->clientPort = clientPort;
    
    gameServerInfo->roomCount = 0;
    
    gameServerInfo->connectInfo = connectInfo;
    connectInfo->userData = (void*)gameServerInfo;
    
    gameServerInfoList.push_back(gameServerInfo);
    
    return true;
}

bool ServerInfoManager::removeGameServer(int gameServerNo)
{
    for(gameServerInfoListItr = gameServerInfoList.begin(); gameServerInfoListItr != gameServerInfoListItr; gameServerInfoListItr++)
    {
        GameServerInfo* gameServerInfo = *gameServerInfoListItr;
        
        if(gameServerInfo->gameServerNo == gameServerNo)
        {
            delete gameServerInfo;
            gameServerInfoList.erase(gameServerInfoListItr);
            
            return true;
        }
    }
    
    return false;
}


GameServerInfo* ServerInfoManager::getGameServerInfo(int gameServerNo)
{
    for(gameServerInfoListItr = gameServerInfoList.begin(); gameServerInfoListItr != gameServerInfoList.end(); gameServerInfoListItr++)
    {
        GameServerInfo* gameServerInfo = *gameServerInfoListItr;
        
        if(gameServerInfo->gameServerNo == gameServerNo)
        {
            return gameServerInfo;
        }
    }
    
    return NULL;
}


GameServerInfo* ServerInfoManager::getFreeGameServerInfo()
{
    GameServerInfo* gameServerInfo1 = gameServerInfoList[0];
    GameServerInfo* gameServerInfo2 = NULL;
    
    for(gameServerInfoListItr = gameServerInfoList.begin(); gameServerInfoListItr != gameServerInfoList.end(); gameServerInfoListItr++)
    {
        gameServerInfo2 = *gameServerInfoListItr;
        
        if(gameServerInfo2->roomCount < gameServerInfo1->roomCount)
        {
            gameServerInfo1 = gameServerInfo2;
        }
    }
    
    return gameServerInfo1;
}


void ServerInfoManager::increaseGameServerRoomCount(int gameServerNo)
{
    for(gameServerInfoListItr = gameServerInfoList.begin(); gameServerInfoListItr != gameServerInfoList.end(); gameServerInfoListItr++)
    {
        GameServerInfo* gameServerInfo = *gameServerInfoListItr;
        
        if(gameServerInfo->gameServerNo == gameServerNo)
        {
            gameServerInfo->roomCount++;
        }
    }
}


void ServerInfoManager::decreaseGameServerRoomCount(int gameServerNo)
{
    for(gameServerInfoListItr = gameServerInfoList.begin(); gameServerInfoListItr != gameServerInfoList.end(); gameServerInfoListItr++)
    {
        GameServerInfo* gameServerInfo = *gameServerInfoListItr;
        
        if(gameServerInfo->gameServerNo == gameServerNo)
        {
            gameServerInfo->roomCount--;
        }
    }
}
