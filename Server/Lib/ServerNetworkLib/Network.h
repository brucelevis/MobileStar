#ifndef __NETWORK_H__
#define __NETWORK_H__


#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <deque>
#include <vector>
#include <boost/pool/object_pool.hpp>
#include <boost/unordered_map.hpp>

#include "BasicDefines.h"
#include "WorkerThread.h"

#if OS_PLATFORM == PLATFORM_LINUX
#include <sys/epoll.h>

#elif OS_PLATFORM == PLATFORM_MAC
#include <sys/event.h>
#endif


#define MAX_IP_LEN 15
#define EVENT_BUFFER_SIZE 50
#define MAX_CONNECT_SIZE 100
#define RECV_BUF 5000

typedef int8_t dataType_t;
typedef uint64_t dataSize_t;

typedef int stringSize_t;


class Timer
{
protected:
    Timer(int _timerNo, int _interval, int _finishCount);
    
public:
    virtual void ProcessTimerStart() = 0;
    virtual void ProcessTimerInterval() = 0;
    virtual void ProcessTimerFinish() = 0;
    
    friend class Network;
    
protected:
    int timerNo;
    long startTime;
    int interval;
    int finishCount;
};

#pragma pack(push, 1)
struct DataHeader
{
    dataType_t dataType;
    dataSize_t dataSize;
};
#pragma pack(pop)

class CharString
{
public:
    CharString();
    ~CharString();
    void clear();
    void deepCopy(stringSize_t _stringSize, const char* _string);
    void shallowCopy(stringSize_t _stringSize, char* _string);
    
    stringSize_t stringSize;
    char* string;
};


class ConnectInfo
{
public:
    ConnectInfo();
    ~ConnectInfo();
    void clear();
    
    
    int fd;
    int serverModule;
    char flags;
    int sendPingCount;
    time_t lastPingTime;
    void* userData;
    CharString tempBufferInfo;
    std::deque<CharString*> tempDataQueue;
};

class ListenerInfo
{
public:
    ListenerInfo();
    ~ListenerInfo();
    
    int fd;
    int serverModule;
    char flags;
    long sendPingInterval;
    int disconnectPingCount;
    long lastPingCheckTime;
//    boost::object_pool<ConnectInfo> connectInfoPool;
    boost::unordered_map<int, ConnectInfo*> connectInfoMap;
};


class DataPacket
{
public:
    DataPacket();
    ~DataPacket();
    void clear();
    
    
    int receiveType;
    ConnectInfo* connectInfo;
    CharString dataInfo;
};


enum SERVER_TYPE
{
    SERVER_TYPE_SERVER = 0,
    SERVER_TYPE_SIBLE,
    SERVER_TYPE_CLIENT,
};

enum DATA_TYPE
{
    DATA_TYPE_REQ = 64,
    DATA_TYPE_PING_NOTIFY,
    DATA_TYPE_PING_OK,
};

enum RECEIVE_TYPE
{
    RECEIVE_TYPE_RECEIVE = 0,
    RECEIVE_TYPE_CONNECT,
    RECEIVE_TYPE_DISCONNECT,
};

struct NetworkInfo
{
    int module;
    int type;
    char ip[MAX_IP_LEN + 1];
    int port;
    long sendPingInterval;
    int disconnectPingCount;
};


class Network
{
public:
	Network();
	~Network();
	bool Initialize(const NetworkInfo* _networkInfoList, int _networkInfoCount, int _workThreadCount, WorkerThread** _workerThreadArray);
    bool AddNetworkInfo(const NetworkInfo* _networkInfo);
    bool AddTimer(Timer* timer);
	void ProcessEvent();
    
#if THREAD_TYPE == SINGLE_THREAD
    void sendPacket(ConnectInfo* connectInfo, const char* data, int dataSize);
#else
    void sendPacket(int threadId, const ConnectInfo* connectInfo, const char* data, int dataSize);
    void finishProcessing(int threadId, const ConnectInfo* connectInfo);
    void sendDataToWorkerThread(int receiveType, ConnectInfo* const _connectInfo, const char* _data = NULL, int _dataSize = 0);
    void finishProcessingCallback(ConnectInfo* connectInfo);
#endif

private:
    void sendData(ConnectInfo* connectInfo, const char* data, int dataSize);

	int CreateTCPServerSocket(const char* ip, unsigned short port);
    int CreateTCPClientSocket(const char* ip, unsigned short port);
    bool AddServerTypeNetworkInfo(const NetworkInfo* _networkInfo);
    bool AddClientTypeNetworkInfo(const NetworkInfo* _networkInfo);
    
    void disconnectWithSocket(ConnectInfo* connectInfo);
    
    void initThreadPipe(ThreadPipe* threadPipe);
    
    long getCustomCurrentTime();
    void pingCheck(ListenerInfo* listenerInfo);
private:
	int eventFd;
	int clntFd;
	struct sockaddr_in clntaddr;
	int clntaddrLen;
    int listenSocketCount;
	
    std::vector<ListenerInfo*> listenerInfoVector;
    std::vector<ConnectInfo*> ServerConnectInfoVector;
    std::vector<Timer*> timerQueue;
    

	char recvBuffer[RECV_BUF];
    int workerThreadCount;
    WorkerThread** workerThreadArray;

    struct timespec wait;
    
    long lastPingCheckTime;
    long nextPingCheckTime;
    
    boost::object_pool<ConnectInfo> connectInfoPool;
    boost::object_pool<DataPacket> dataPacketPool;
    
#if OS_PLATFORM == PLATFORM_LINUX
    
    struct epoll_event* event;
    struct epoll_event connectEvent;
    
#elif OS_PLATFORM == PLATFORM_MAC
    
    struct kevent* event;
    struct kevent connectEvent;
#else
    
#endif
    
};

#endif //_NETWORK_H__