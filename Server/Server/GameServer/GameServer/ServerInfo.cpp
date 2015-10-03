
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
    createLobbyServerNo = 1;
    lobbyServerInfoList.clear();
    return true;
}


bool ServerInfoManager::addLobbyServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort)
{
    LobbyServerInfo* lobbyServerInfo = new LobbyServerInfo();
    
    lobbyServerInfo->lobbyServerNo = createLobbyServerNo++;
    
    memcpy(lobbyServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    lobbyServerInfo->clientPort = clientPort;
    
    lobbyServerInfo->connectInfo = connectInfo;
    connectInfo->userData = (void*)lobbyServerInfo;
    
    lobbyServerInfoList.push_back(lobbyServerInfo);
    
    return true;
}

bool ServerInfoManager::removeLobbyServer(int lobbyServerNo)
{
    for(lobbyServerInfoListItr = lobbyServerInfoList.begin(); lobbyServerInfoListItr != lobbyServerInfoListItr; lobbyServerInfoListItr++)
    {
        LobbyServerInfo* lobbyServerInfo = *lobbyServerInfoListItr;
        
        if(lobbyServerInfo->lobbyServerNo == lobbyServerNo)
        {
            delete lobbyServerInfo;
            lobbyServerInfoList.erase(lobbyServerInfoListItr);
            
            return true;
        }
    }
    
    return false;
}