
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
    createGameServerNo = 1;
    gameServerInfoList.clear();
    return true;
}


bool ServerInfoManager::addGameServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort)
{
    GameServerInfo* gameServerInfo = new GameServerInfo();
    
    gameServerInfo->gameServerNo = createGameServerNo++;
    
    memcpy(gameServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    gameServerInfo->clientPort = clientPort;
    
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



