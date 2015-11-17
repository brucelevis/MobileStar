
// create by yong chan kim
// since 2014.7.29


#ifndef __SERVER_INFO_H__
#define __SERVER_INFO_H__

#include <stdint.h>
#include <vector>
#include "BasicPacket.h"
#include "Network.h"

struct CacheServerInfo
{
    ConnectInfo* connectInfo;
};

struct ChattingServerInfo
{
    ConnectInfo* connectInfo;
    
    char clientIp[MAX_IP_ADDRESS_LEN];
    int clientPort;
};

struct GameServerInfo
{
    ConnectInfo* connectInfo;
    
    int gameServerNo;
    char clientIp[MAX_IP_ADDRESS_LEN];
    int clientPort;
    
    int roomCount;
};


class ServerInfoManager
{
public:
	ServerInfoManager();
	bool initialize();
    
    bool addCacheServer(ConnectInfo* connectInfo);
    bool addChattingServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort);
    bool removeChattingServer();
    CacheServerInfo* getCacheServerInfo() { return cacheServerInfo; }
    ChattingServerInfo* getChattingServerInfo() { return chattingServerInfo; }
    
    bool addGameServer(ConnectInfo* connectInfo, char* clientIp, uint16_t clientPort);
    bool removeGameServer(int gameServerNo);
    
    GameServerInfo* getGameServerInfo(int gameServeNo);
    GameServerInfo* getFreeGameServerInfo();
    void increaseGameServerRoomCount(int gameServerNo);
    void decreaseGameServerRoomCount(int gameServerNo);
    

	~ServerInfoManager();

private:
    int createGameServerNo;
    CacheServerInfo* cacheServerInfo;
    ChattingServerInfo* chattingServerInfo;
    std::vector<GameServerInfo*> gameServerInfoList;
    std::vector<GameServerInfo*>::iterator gameServerInfoListItr;
};

#endif //__SERVER_INFO_H__
