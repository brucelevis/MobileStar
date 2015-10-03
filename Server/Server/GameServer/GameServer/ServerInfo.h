
// create by yong chan kim
// since 2014.7.29


#ifndef __SERVER_INFO_H__
#define __SERVER_INFO_H__

#include <stdint.h>
#include <vector>
#include "BasicPacket.h"
#include "Network.h"

struct LobbyServerInfo
{
    ConnectInfo* connectInfo;
    
    int lobbyServerNo;
    char clientIp[MAX_IP_ADDRESS_LEN];
    int clientPort;
};


class ServerInfoManager
{
public:
	ServerInfoManager();
	bool initialize();
    bool addLobbyServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort);
    bool removeLobbyServer(int lobbyServerNo);
    
    LobbyServerInfo* getLobbyServerInfo() { return lobbyServerInfoList[0]; }
    

	~ServerInfoManager();

private:
    int createLobbyServerNo;
    std::vector<LobbyServerInfo*> lobbyServerInfoList;
    std::vector<LobbyServerInfo*>::iterator lobbyServerInfoListItr;
};

#endif //__SERVER_INFO_H__
