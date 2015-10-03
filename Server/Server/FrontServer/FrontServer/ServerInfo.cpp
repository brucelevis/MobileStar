
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
    lobbyServerInfoList.clear();
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
    
    memcpy(lobbyServerInfo->gameIp, gameIp, MAX_IP_ADDRESS_LEN);
    lobbyServerInfo->gamePort = gamePort;
    memcpy(lobbyServerInfo->clientIp, clientIp, MAX_IP_ADDRESS_LEN);
    lobbyServerInfo->clientPort = clientPort;
    
    lobbyServerInfo->connectInfo = connectInfo;
    connectInfo->userData = (void*)lobbyServerInfo;
    
    lobbyServerInfoList.push_back(lobbyServerInfo);
    
    return true;
}