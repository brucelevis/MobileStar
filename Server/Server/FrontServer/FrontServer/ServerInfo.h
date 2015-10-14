
// create by yong chan kim
// since 2014.7.29


#ifndef __SERVER_INFO_H__
#define __SERVER_INFO_H__

#include <stdint.h>
#include <vector>
#include "BasicPacket.h"
#include "Network.h"


struct ChattingServerInfo
{
    ConnectInfo* connectInfo;
    char lobbyIp[MAX_IP_ADDRESS_LEN];
    uint16_t lobbyPort;
    
    char gameIp[MAX_IP_ADDRESS_LEN];
    uint16_t gamePort;
    
    char clientIp[MAX_IP_ADDRESS_LEN];
    uint16_t clientPort;
};

struct LobbyServerInfo
{
    int lobbyNo;
    ConnectInfo* connectInfo;
    
    char gameIp[MAX_IP_ADDRESS_LEN];
    uint16_t gamePort;
    
    char clientIp[MAX_IP_ADDRESS_LEN];
    uint16_t clientPort;
};

struct GameServerInfo
{
    ConnectInfo* connectInfo;
};


class ServerInfoManager
{
public:
	ServerInfoManager();
	bool initialize();
    bool addChattingServer(ConnectInfo* connectInfo, char* _lobbyIp, uint16_t lobbyPort, char* _gameIp, uint16_t gamePort, char* _clientIp, uint16_t clientPort);
    bool addLobbyServer(ConnectInfo* connectInfo, char* _gameIp, uint16_t gamePort, char* _clientIp, uint16_t clientPort);
    //bool AddChattingServer(char* _clientIp, uint16_t clientPort);
    
    ChattingServerInfo* getChattingServerInfo() { return chattingServerInfo; }
    LobbyServerInfo* getRandomLobbyServerInfo() { return lobbyServerInfoList[0]; }
    LobbyServerInfo* getLobbyServerInfo(int lobbyNo);
    
    int getLobbyCount() { return (int)lobbyServerInfoList.size(); }
    

	~ServerInfoManager();

private:
    ChattingServerInfo* chattingServerInfo;
    std::vector<LobbyServerInfo*> lobbyServerInfoList;
    std::vector<GameServerInfo*> gameServerInfoList;
    
    int lobbyNoCreator;
};

#endif //__SERVER_INFO_H__
