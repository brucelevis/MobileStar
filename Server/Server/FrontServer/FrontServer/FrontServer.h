
//
// CenterServer.h
//
// create by kimyc on 2014.6.28
//


#ifndef __FRONT_SERVER_H__
#define __FRONT_SERVER_H__

#include <stddef.h>


class ServerInfoManager;
class UserManager;
class FrontData;

class Network;
class Log;

class FrontServer
{
private:
    FrontServer() {};
    static FrontServer* m_instance;
    
public:
    static FrontServer* getInstance() {
        if (m_instance == NULL)
            m_instance = new FrontServer();
        return m_instance;
    }
    
public:
    bool initialize(int workerThreadCount);
    void run();
    
public:
    Network* network;
    Log* log;
    
	FrontData* frontData;
	ServerInfoManager* serverInfoMgr;
	UserManager* userMgr;

private:
	long _prevTime;
};

#endif //__FRONT_SERVER_H__


