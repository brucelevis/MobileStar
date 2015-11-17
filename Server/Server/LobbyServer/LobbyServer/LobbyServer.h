
//
// CenterServer.h
//
// create by kimyc on 2014.6.28
//


#ifndef __LOBBY_SERVER_H__
#define __LOBBY_SERVER_H__

#include <stddef.h>


class ChannelManager;
class RoomManager;
class UserManager;
class ServerInfoManager;
class QuickPlayManager;
class ClanManager;

class LobbyData;

class Network;
class Log;

class LobbyServer
{
private:
    LobbyServer() {};
    static LobbyServer* m_instance;
    
public:
    static LobbyServer* getInstance() {
        if (m_instance == NULL)
            m_instance = new LobbyServer();
        return m_instance;
    }
    
public:
    bool initialize(int workerThreadCount);
    void run();
    
public:
    Network* network;
    Log* log;
    
	LobbyData* lobbyData;
	ChannelManager* channelMgr;
    RoomManager* roomMgr;
	UserManager* userMgr;
    ServerInfoManager* serverInfoMgr;
    QuickPlayManager* quickPlayMgr;
    ClanManager* clanMgr;
    
private:
	long _prevTime;
};

#endif //__LOBBY_SERVER_H__


