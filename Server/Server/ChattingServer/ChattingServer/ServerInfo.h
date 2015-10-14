
// create by yong chan kim
// since 2014.7.29


#ifndef __SERVER_INFO_H__
#define __SERVER_INFO_H__

#include <stdint.h>
#include <vector>
#include "BasicPacket.h"
#include "Network.h"


struct GameServerInfo
{
    ConnectInfo* connectInfo;
    
    int gameServerNo;
    char clientIp[MAX_IP_ADDRESS_LEN];
    int clientPort;
};


class ServerInfoManager
{
public:
	ServerInfoManager();
	bool initialize();
    bool addGameServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort);
    bool removeGameServer(int gameServerNo);
    
    GameServerInfo* getGameServerInfo(int gameServeNo);
    GameServerInfo* getFreeGameServerInfo() { return gameServerInfoList[0]; }
    

	~ServerInfoManager();

private:
    int createGameServerNo;
    std::vector<GameServerInfo*> gameServerInfoList;
    std::vector<GameServerInfo*>::iterator gameServerInfoListItr;
};

#endif //__SERVER_INFO_H__
