
//
// CenterServer.h
//
// create by kimyc on 2014.6.28
//


#ifndef __CACHE_SERVER_H__
#define __CACHE_SERVER_H__

#include <stddef.h>


class ServerInfoManager;
class UserManager;
class CacheData;

class Network;
class Log;

class ClanManager;

class GameDBChannel;

class CacheServer
{
private:
    CacheServer() {};
    static CacheServer* m_instance;
    
public:
    static CacheServer* getInstance() {
        if (m_instance == NULL)
            m_instance = new CacheServer();
        return m_instance;
    }
    
public:
    bool initialize(int workerThreadCount);
    void run();
    
public:
    Network* network;
    Log* log;
    
	CacheData* cacheData;
	ServerInfoManager* serverInfoMgr;
	UserManager* userMgr;
    ClanManager* clanMgr;
    GameDBChannel* gameDBChannel;

private:
	long _prevTime;
};

#endif //__CACHE_SERVER_H__


