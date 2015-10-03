
//
// CenterServer.h
//
// create by kimyc on 2014.6.28
//


#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include <stddef.h>


class UserManager;
class RoomManager;
class GameData;
class ServerInfoManager;

class Network;
class Log;

class GameServer
{
private:
    GameServer() {};
    static GameServer* m_instance;
    
public:
    static GameServer* getInstance() {
        if (m_instance == NULL)
            m_instance = new GameServer();
        return m_instance;
    }
    
public:
    bool initialize(int workerThreadCount);
    void run();
    
public:
    Network* network;
    Log* log;
    
	GameData* gameData;
	RoomManager* roomMgr;
	UserManager* userMgr;
    ServerInfoManager* serverInfoMgr;

private:
	long _prevTime;
};

#endif //__GAME_SERVER_H__


