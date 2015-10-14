
//
// ChattingServer.h
//
// create by kimyc on 2014.6.28
//


#ifndef __CHATTING_SERVER_H__
#define __CHATTING_SERVER_H__

#include <stddef.h>


class LObjectManager;
class UserManager;

class ChattingData;

class Network;
class Log;

class ChattingServer
{
private:
    ChattingServer() {};
    static ChattingServer* m_instance;
    
public:
    static ChattingServer* getInstance() {
        if (m_instance == NULL)
            m_instance = new ChattingServer();
        return m_instance;
    }
    
public:
    bool initialize(int workerThreadCount);
    void run();
    
public:
    Network* network;
    Log* log;
    
    ChattingData* chattingData;
    LObjectManager* lObjectMgr;
    UserManager* userMgr;
    
private:
    long _prevTime;
};

#endif //__CHATTING_SERVER_H__


