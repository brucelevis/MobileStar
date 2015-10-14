
#include "Network.h"

#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Log.h"

#define FLAG_PROCESSING     0x01 // 00000001
#define FLAG_DISCONNECTED   0x02 // 00000010

void* WorkerThreadFunction(void *data)
{
    WorkerThread* wt = (WorkerThread*)data;
    
    wt->run();
    
    return NULL;
}

///////////////////

Timer::Timer(int _timerNo, int _interval, int _finishCount)
{
    timerNo = _timerNo;
    interval = _interval;
    finishCount = _finishCount;
    startTime = 0;
}


////////////////////////CharString
CharString::CharString()
{
    stringSize = 0;
    string = NULL;
}


CharString::~CharString()
{
    if(stringSize != 0)
        free(string);
}


void CharString::clear()
{
    if(stringSize != 0)
        free(string);
    stringSize = 0;
}


void CharString::deepCopy(stringSize_t _stringSize, const char* _string)
{
    stringSize = _stringSize;
    string = (char*)malloc(stringSize);
    memcpy(string, _string, stringSize);
}


void CharString::shallowCopy(stringSize_t _stringSize, char* _string)
{
    stringSize = _stringSize;
    string = _string;
}
/////////////////////////////////////



////////////////////////ConnectInfo
ConnectInfo::ConnectInfo()
{
    fd = 0;
    serverModule = 0;
    flags = 0;
    sendPingCount = 0;
    lastPingTime = 0;
    userData = NULL;
    tempBufferInfo.clear();
    tempDataQueue.clear();
}

ConnectInfo::~ConnectInfo()
{
    if(userData != NULL)
    {
        ErrorLog("userData not NULL");
    }
    
    userData = NULL;
    
    tempBufferInfo.clear();
    
    for(int i = 0; i <tempDataQueue.size(); i++)
    {
        CharString* tempDataInfo = tempDataQueue.front();
        tempDataQueue.pop_front();
        delete tempDataInfo;
    }
}

void ConnectInfo::clear()
{
    fd = 0;
    serverModule = 0;
    flags = 0;
    sendPingCount = 0;
    lastPingTime = 0;
    if(userData != NULL)
    {
        ErrorLog("userData not NULL");
    }
    
    userData = NULL;
    
    tempBufferInfo.clear();
    
    for(int i = 0; i <tempDataQueue.size(); i++)
    {
        CharString* tempDataInfo = tempDataQueue.front();
        tempDataQueue.pop_front();
        delete tempDataInfo;
    }
    
    tempDataQueue.clear();
}
/////////////////////////////////////




////////////////////////ListenerInfo
ListenerInfo::ListenerInfo()
{
    fd = 0;
    serverModule = 0;
    flags = 0;
    sendPingInterval = 0;
    disconnectPingCount = 0;
    lastPingCheckTime = 0;
}

ListenerInfo::~ListenerInfo()
{

}

/////////////////////////////////////




////////////////////////DataPacket
DataPacket::DataPacket()
{
    receiveType = 0;
    connectInfo = NULL;
    dataInfo.clear();
}

DataPacket::~DataPacket()
{
    dataInfo.clear();
}

void DataPacket::clear()
{
    receiveType = 0;
    connectInfo = NULL;
    dataInfo.clear();
}
/////////////////////////////////////






long Network::getCustomCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);    // milliseconds 단위로 변환
}

Network::Network()
{
}


Network::~Network()
{
}


bool Network::Initialize(const NetworkInfo* _networkInfoList, int _networkInfoCount, int _workerThreadCount, WorkerThread** _workerThreadArray)
{
    listenSocketCount = 0;
    
    signal(SIGPIPE, SIG_IGN);
    
    
#if OS_PLATFORM == PLATFORM_LINUX
    
    eventFd = epoll_create(EVENT_BUFFER_SIZE);
    if (eventFd < 0) {
        perror("epoll create error.");
        exit(2);
    }
    
    event = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EVENT_BUFFER_SIZE);
    
    memset(&connectEvent, 0, sizeof(struct epoll_event));
    
#elif OS_PLATFORM == PLATFORM_MAC
    
    eventFd = kqueue();
    if (eventFd < 0) {
        ErrorLog("kqueue create error.");
        return false;
    }
    
    event = (struct kevent*)malloc(sizeof(struct kevent)*EVENT_BUFFER_SIZE);
 
#endif
    
    
    memset(recvBuffer, 0, RECV_BUF);

    clntaddrLen = sizeof(clntaddr);
    
#if THREAD_TYPE == SINGLE_THREAD
    workerThreadCount = 0;
    
    workerThreadArray = _workerThreadArray;
    
    if (workerThreadArray[0]->initialize() == false)
    {
        ErrorLog("worker thread error");
        return false;
    }
#else
    
    workerThreadCount = _workerThreadCount;
    
    workerThreadArray = _workerThreadArray;
    
    for (int i = 0; i < workerThreadCount; i++)
    {
        if (workerThreadArray[i]->initialize() == false)
        {
            ErrorLog("worker thread error");
            return false;
        }
        
        if (pthread_create(workerThreadArray[i]->getTid(), NULL, WorkerThreadFunction, (void*)workerThreadArray[i]) != 0)
        {
            ErrorLog("thread create error : ");
            return false;
        }
        
        initThreadPipe(&(workerThreadArray[i]->threadPipe));
    }
#endif

    
    for(int i=0; i < _networkInfoCount; i++)
    {
        int type = _networkInfoList[i].type;
        
        if(type == SERVER_TYPE_SERVER)
        {
            if(AddServerTypeNetworkInfo(&_networkInfoList[i]) == false)
            {
                ErrorLog("AddServerType NetworkInfo error");
                return false;
            }
        }
        else if(type == SERVER_TYPE_CLIENT)
        {
            if(AddClientTypeNetworkInfo(&_networkInfoList[i]) == false)
            {
                ErrorLog("AddClientType NetworkInfo error");
                return false;
            }
        }
        else
        {
            ErrorLog("type error - %d", type);
            return false;
        }
    }
    
    
	return true;
}


void Network::initThreadPipe(ThreadPipe* threadPipe)
{
    int pipefds[2] = {};
    pipe(pipefds);
    threadPipe->readPipe = pipefds[0];
    threadPipe->writePipe = pipefds[1];
    
    // make read-end non-blocking
    int flags = fcntl(threadPipe->readPipe, F_GETFL, 0);
    fcntl(threadPipe->writePipe, F_SETFL, flags|O_NONBLOCK);
    
#if OS_PLATFORM == PLATFORM_LINUX
    // add the read end to the epoll
    connectEvent.events = EPOLLIN;
    connectEvent.data.ptr = threadPipe;
    epoll_ctl(eventFd, EPOLL_CTL_ADD, threadPipe->readPipe, &connectEvent);
    

#elif OS_PLATFORM == PLATFORM_MAC
    
    EV_SET(&connectEvent, threadPipe->readPipe, EVFILT_READ, EV_ADD, 0, 0, (void*)threadPipe);
    
    if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
    {
        ErrorLog("kevent init error");
    }
    
#endif
    
}


bool Network::AddNetworkInfo(const NetworkInfo* _networkInfo)
{
    if(_networkInfo->type != SERVER_TYPE_CLIENT)
    {
        if(AddServerTypeNetworkInfo(_networkInfo) == false)
        {
            ErrorLog("AddServerType NetworkInfo error");
            return false;
        }
    }
    else
    {
        if(AddClientTypeNetworkInfo(_networkInfo) == false)
        {
            ErrorLog("AddClientType NetworkInfo error");
            return false;
        }
    }
    
    return true;
}

bool Network::AddTimer(Timer *timer)
{
    timer->startTime = getCustomCurrentTime();
    timerQueue.push_back(timer);
    
    return true;
}

bool Network::AddServerTypeNetworkInfo(const NetworkInfo* _networkInfo)
{
    ListenerInfo* listenerInfo = new ListenerInfo();
    listenerInfo->fd = CreateTCPServerSocket(_networkInfo->ip, _networkInfo->port);
    if (listenerInfo->fd < 0)
    {
        delete listenerInfo;
        ErrorLog("create sock error");
        return false;
    }
    
    listenerInfo->serverModule = _networkInfo->module;
    if(_networkInfo->type == SERVER_TYPE_SERVER)
    {
        listenerInfo->sendPingInterval = _networkInfo->sendPingInterval;
        listenerInfo->disconnectPingCount = _networkInfo->disconnectPingCount;
        listenerInfo->lastPingCheckTime = getCustomCurrentTime();
    }
    else
    {
        listenerInfo->sendPingInterval = 0;
        listenerInfo->disconnectPingCount = 0;
        listenerInfo->lastPingCheckTime = 0;
    }
    
    
#if OS_PLATFORM == PLATFORM_LINUX
    connectEvent.events = EPOLLIN;
    connectEvent.data.ptr = (void*)listenerInfo;
    epoll_ctl(eventFd, EPOLL_CTL_ADD, listenerInfo->fd, &connectEvent);
    
#elif OS_PLATFORM == PLATFORM_MAC
    
    EV_SET(&connectEvent, listenerInfo->fd, EVFILT_READ, EV_ADD, 0, 0, (void*)listenerInfo);
    
    if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
    {
        ErrorLog("kevent init error");
        return false;
    }
    
#endif
    
    
    listenerInfoVector.push_back(listenerInfo);
    
    listenSocketCount++;
    return true;
}

bool Network::AddClientTypeNetworkInfo(const NetworkInfo* _networkInfo)
{
    int fd = CreateTCPClientSocket(_networkInfo->ip, _networkInfo->port);
    if(fd < 0)
    {
        ErrorLog("create sock error");
        return false;
    }
    
    ConnectInfo* connectInfo = new ConnectInfo();
    connectInfo->fd = fd;
    connectInfo->serverModule = _networkInfo->module;
    if(connectInfo == NULL)
    {
        ErrorLog("add pool error");
        return false;
    }
    
    ServerConnectInfoVector.push_back(connectInfo);
    
    
#if OS_PLATFORM == PLATFORM_LINUX
    
    connectEvent.events = EPOLLIN;
    connectEvent.data.ptr = (void*)connectInfo;
    epoll_ctl(eventFd, EPOLL_CTL_ADD, connectInfo->fd, &connectEvent);
    
    
#elif OS_PLATFORM == PLATFORM_MAC
    
    EV_SET(&connectEvent, connectInfo->fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void*)connectInfo);
    
    if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
    {
        ErrorLog("kevent init error");
        return false;
    }
    
#endif
    

#if THREAD_TYPE == SINGLE_THREAD
    workerThreadArray[0]->connected(connectInfo);
#else
    connectInfo->flags |= FLAG_PROCESSING;
    sendDataToWorkerThread(RECEIVE_TYPE_CONNECT, connectInfo);
#endif
    
    return true;
}


void Network::ProcessEvent()
{
	int eventCnt;
    long timeInterval;
    
    struct timespec waitTime;
    
    long currentTime = getCustomCurrentTime();
    
    nextPingCheckTime = LONG_MAX;
    
    std::vector<ListenerInfo*>::iterator lvitr;
    for(lvitr = listenerInfoVector.begin(); lvitr != listenerInfoVector.end(); lvitr++) //get next ping check time
    {
        ListenerInfo* listenerInfo = *lvitr;
        
        if (listenerInfo->sendPingInterval != 0) {
            listenerInfo->lastPingCheckTime = currentTime;
            if(nextPingCheckTime > listenerInfo->sendPingInterval + currentTime)
            {
                nextPingCheckTime = listenerInfo->sendPingInterval + currentTime;
            }
        }
    }
    
    std::vector<Timer*>::iterator timerItr;
    for(timerItr = timerQueue.begin(); timerItr != timerQueue.end(); timerItr++) //get next ping check time
    {
        Timer* timer = *timerItr;
        
        timer->startTime = currentTime;
        if(nextPingCheckTime > timer->interval + currentTime)
        {
            nextPingCheckTime = timer->interval + currentTime;
        }
    }
    
    
    
	while(true)
	{
        currentTime = getCustomCurrentTime();
        
        if(nextPingCheckTime < currentTime) // ping check
        {
            nextPingCheckTime = LONG_MAX;
            
            for(lvitr = listenerInfoVector.begin(); lvitr != listenerInfoVector.end(); lvitr++)
            {
                ListenerInfo* listenerInfo = *lvitr;
                
                if(listenerInfo->sendPingInterval != 0)
                {
                    if(listenerInfo->lastPingCheckTime + listenerInfo->sendPingInterval < currentTime)
                    {
                        pingCheck(listenerInfo);
                        listenerInfo->lastPingCheckTime = currentTime;
                    }
                    
                    
                    if(nextPingCheckTime > listenerInfo->sendPingInterval + listenerInfo->lastPingCheckTime)
                    {
                        nextPingCheckTime = listenerInfo->sendPingInterval + listenerInfo->lastPingCheckTime;
                    }
                }
            }
            
            for(timerItr = timerQueue.begin(); timerItr != timerQueue.end(); timerItr++)
            {
                Timer* timer = *timerItr;

                if(timer->startTime + timer->interval < currentTime)
                {
                    timer->ProcessTimerInterval();
                    timer->startTime = currentTime;
                }
                
                
                if(nextPingCheckTime > timer->interval + timer->startTime)
                {
                    nextPingCheckTime = timer->interval + timer->startTime;
                }
            }
        }

        
        
#if OS_PLATFORM == PLATFORM_LINUX
        
        if(nextPingCheckTime == LONG_MAX)
        {
            eventCnt = epoll_wait(eventFd, event, EVENT_BUFFER_SIZE, -1);
        }
        else
        {
            timeInterval = nextPingCheckTime - currentTime;
            eventCnt = epoll_wait(eventFd, event, EVENT_BUFFER_SIZE, timeInterval);
        }
        
        if (eventCnt < 0)
        {
            ErrorLog("kevent wait fail.");
            continue;
        }
        else if(eventCnt == 0)
        {
            continue;
        }
        
        for (int i = 0; i < eventCnt; i++)
        {
            if (true) //if threadEvent == false
            {
                bool isServer = false;
                for(int j = 0; j < listenSocketCount; j++)
                {
                    if(event[i].data.ptr == listenerInfoVector.at(j)) // Server sock
                    {
        
#elif OS_PLATFORM == PLATFORM_MAC
        
        if(nextPingCheckTime == LONG_MAX)
        {
            eventCnt = kevent(eventFd, NULL, 0, event, EVENT_BUFFER_SIZE, NULL);
        }
        else
        {
            timeInterval = nextPingCheckTime - currentTime;
            waitTime.tv_sec = timeInterval / 1000;
            waitTime.tv_nsec = (timeInterval % 1000) * 1000000;
            
            eventCnt = kevent(eventFd, NULL, 0, event, EVENT_BUFFER_SIZE, &waitTime);
        }
        
        if (eventCnt < 0)
        {
            ErrorLog("kevent wait fail.");
            continue;
        }
        else if(eventCnt == 0)
        {
            continue;
        }
        
        for (int i = 0; i < eventCnt; i++)
        {
            if (event[i].filter == EVFILT_READ)
            {
                bool isServer = false;
                
                for(int j = 0; j < listenSocketCount; j++)
                {
                    if(event[i].ident == listenerInfoVector.at(j)->fd) // Server sock
                    {
        
#endif

                        isServer = true;
                        clntFd = accept(listenerInfoVector.at(j)->fd, (struct sockaddr *)&clntaddr, (socklen_t*)&clntaddrLen);
                        if (clntFd < 0)
                        {
                            ErrorLog("accept fail.");
                            break;
                        }
                        
                        ConnectInfo* connectInfo = connectInfoPool.construct();
                        
                        connectInfo->fd = clntFd;
                        connectInfo->serverModule = listenerInfoVector.at(j)->serverModule;
                        
                        int status = fcntl(clntFd, F_SETFL, fcntl(clntFd, F_GETFL, 0) | O_NONBLOCK);
                        
                        if (status == -1)
                        {
                            perror("calling fcntl");
                        }
                        
                        // LINGER 구조체의 값 설정
                        struct linger ling = {0,};
                        ling.l_onoff = 1;   // LINGER 옵션 사용 여부
                        ling.l_linger = 0;  // LINGER Timeout 설정
                        
                        // LINGER 옵션을 Socket에 적용
                        setsockopt(clntFd, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));

                        
                        
#if OS_PLATFORM == PLATFORM_LINUX
                        
                        connectEvent.events = EPOLLIN;
                        connectEvent.data.ptr = (void*)connectInfo;
                        epoll_ctl(eventFd, EPOLL_CTL_ADD, connectInfo->fd, &connectEvent);
                        
#elif OS_PLATFORM == PLATFORM_MAC
                        
                        EV_SET(&connectEvent, clntFd, EVFILT_READ, EV_ADD, 0, 0, (void*)connectInfo);
                        
                        if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
                        {
                            ErrorLog("kevent init error");
                            break;
                        }
                        
#endif
                        
                        if (listenerInfoVector.at(j)->connectInfoMap.insert(boost::unordered_map<int, ConnectInfo*>::value_type(connectInfo->fd, connectInfo)).second == false)
                        {
                            ErrorLog("insert error");
                            connectInfoPool.destroy(connectInfo);
                            break;
                        }
                        
#if THREAD_TYPE == SINGLE_THREAD
                        workerThreadArray[0]->connected(connectInfo);
#else
                        connectInfo->flags |= FLAG_PROCESSING;
                        sendDataToWorkerThread(RECEIVE_TYPE_CONNECT, connectInfo);
#endif
                        break;
                    }
                }
                
                
                if(isServer) continue;
                
                ConnectInfo* connectInfo;
#if THREAD_TYPE == SINGLE_THREAD
#else
                int workerThreadCnt;
                for(workerThreadCnt = 0; workerThreadCnt < workerThreadCount; workerThreadCnt++)
                {
#if OS_PLATFORM == PLATFORM_LINUX
                    if(event[i].data.ptr == &(workerThreadArray[workerThreadCnt]->threadPipe))
#elif OS_PLATFORM == PLATFORM_MAC
                    if(event[i].ident == workerThreadArray[workerThreadCnt]->threadPipe.readPipe)
#endif
                    {
                        ssize_t readCount;
                        if((readCount = read(workerThreadArray[workerThreadCnt]->threadPipe.readPipe, (void*)&connectInfo, sizeof(connectInfo))) < 0)
                        {
                            ErrorLog("read error %d %d", readCount, errno);
                            break;
                        }
                        
                        finishProcessingCallback(connectInfo);
                        break;
                    }
                }
                
                if(workerThreadCnt != workerThreadCount) continue;
#endif
                
#if OS_PLATFORM == PLATFORM_LINUX
                
                connectInfo = (ConnectInfo*)event[i].data.ptr;
                clntFd = connectInfo->fd;
                
#elif OS_PLATFORM == PLATFORM_MAC
                
                clntFd = (int)event[i].ident;
                
                connectInfo = (ConnectInfo*)event[i].udata;
                
#endif
                
                int readCnt = 0;
                
                readCnt = (int)recv(clntFd, recvBuffer, RECV_BUF - 1, 0);
                
                
                if (readCnt > 0)
                {
                    char* wholeRecvBuffer = recvBuffer;
                    int wholeRecvBufferSize = readCnt;
                    int recvCnt = readCnt;
                    char* allocateRecvBuffer = NULL;
                    
                    
                    CharString* tempBufferInfo = &connectInfo->tempBufferInfo;
                    
                    int tempBufferSize = tempBufferInfo->stringSize;
                    
                    
                    if(tempBufferSize != 0)
                    {
                        wholeRecvBuffer = (char*)malloc(sizeof(char) * (recvCnt + tempBufferSize));
                        
                        memcpy(wholeRecvBuffer, tempBufferInfo->string, tempBufferSize);
                        memcpy(wholeRecvBuffer + tempBufferSize, recvBuffer, recvCnt);
                        wholeRecvBufferSize += tempBufferInfo->stringSize;
                        
                        connectInfo->tempBufferInfo.stringSize = 0;
                        free(connectInfo->tempBufferInfo.string);
                        connectInfo->tempBufferInfo.string = NULL;
                        
                        allocateRecvBuffer = wholeRecvBuffer;
                    }
                    
                    char* pWholeRecvBuffer = wholeRecvBuffer;
                    int pWholeRecvBufferSize = wholeRecvBufferSize;
                    
                    while(true)
                    {
                        if(pWholeRecvBufferSize < sizeof(dataType_t))
                        {
                            connectInfo->tempBufferInfo.stringSize = wholeRecvBufferSize;
                            connectInfo->tempBufferInfo.string = (char*)malloc(sizeof(char) * wholeRecvBufferSize);
                            memcpy(connectInfo->tempBufferInfo.string, wholeRecvBuffer, wholeRecvBufferSize);
                            
                            break;
                        }
                        
                        ////////////////////////// data type
                        dataType_t flags;
                        memcpy(&flags, pWholeRecvBuffer, sizeof(flags));
                        pWholeRecvBuffer += sizeof(flags);
                        pWholeRecvBufferSize -= sizeof(flags);
                        
                        if(flags == DATA_TYPE_PING_NOTIFY)
                        {
                            dataType_t dataTypeToSend = DATA_TYPE_PING_OK;
                            sendData(connectInfo, (char*)&dataTypeToSend, sizeof(dataTypeToSend));
                            
                            if(pWholeRecvBufferSize == 0)
                            {
                                break;
                            }
                            else if(pWholeRecvBufferSize > 0) // 남은 데이터들 존재함
                            {
                                wholeRecvBufferSize = pWholeRecvBufferSize;
                                wholeRecvBuffer = pWholeRecvBuffer;
                                
                                continue;
                            }
                            else
                            {
                                ErrorLog("pWholeRecvBufferSize - %d", pWholeRecvBufferSize);
                                return ;
                            }
                            
                        }
                        else if (flags == DATA_TYPE_PING_OK)
                        {
                            connectInfo->sendPingCount = 0;
                            
                            if(pWholeRecvBufferSize == 0)
                            {
                                break;
                            }
                            else if(pWholeRecvBufferSize > 0) // 남은 데이터들 존재함
                            {
                                wholeRecvBufferSize = pWholeRecvBufferSize;
                                wholeRecvBuffer = pWholeRecvBuffer;
                                
                                continue;
                            }
                            else
                            {
                                ErrorLog("pWholeRecvBufferSize - %d", pWholeRecvBufferSize);
                                //return ;
                            }
                        }
                        else if(flags == DATA_TYPE_REQ)
                        {
                            if(pWholeRecvBufferSize < sizeof(dataSize_t))
                            {
                                connectInfo->tempBufferInfo.stringSize = wholeRecvBufferSize;
                                connectInfo->tempBufferInfo.string = (char*)malloc(wholeRecvBufferSize);
                                memcpy(connectInfo->tempBufferInfo.string, wholeRecvBuffer, wholeRecvBufferSize);
                                
                                break;
                            }
                            
                            dataSize_t dataSize;
                            memcpy(&dataSize, pWholeRecvBuffer, sizeof(dataSize));
                            pWholeRecvBuffer += sizeof(dataSize);
                            pWholeRecvBufferSize -= sizeof(dataSize);
                        
                        
                            if(dataSize <= pWholeRecvBufferSize)
                            {
#if THREAD_TYPE == SINGLE_THREAD
                                workerThreadArray[0]->receiveData(connectInfo, pWholeRecvBuffer, (int)dataSize);
#else
                                if((connectInfo->flags & FLAG_PROCESSING) != 0)
                                {
                                    CharString* saveData = new CharString();
                                    saveData->deepCopy((int)dataSize, pWholeRecvBuffer);
                                    connectInfo->tempDataQueue.push_back(saveData);
                                }
                                else
                                {
                                    connectInfo->flags |= FLAG_PROCESSING;
                                    sendDataToWorkerThread(RECEIVE_TYPE_RECEIVE, connectInfo, pWholeRecvBuffer, (int)dataSize);
                                }
#endif
                                
                                pWholeRecvBuffer += dataSize;
                                pWholeRecvBufferSize -= dataSize;
                                
                                if(pWholeRecvBufferSize == 0)
                                {
                                    break;
                                }
                                else if(pWholeRecvBufferSize > 0)
                                {
                                    wholeRecvBufferSize = pWholeRecvBufferSize;
                                    wholeRecvBuffer = pWholeRecvBuffer;
                                    
                                    continue;
                                }
                                else
                                {
                                    ErrorLog("pWholeRecvBufferSize - %d", pWholeRecvBufferSize);
                                }
                            }
                            else
                            {
                                connectInfo->tempBufferInfo.deepCopy(wholeRecvBufferSize, wholeRecvBuffer);
                                break;
                            }
                        }
                    
                    }
                    
                    if(allocateRecvBuffer != NULL)
                    {
                        free(allocateRecvBuffer);
                    }
                }
                else if (readCnt == 0)
                {
                    disconnectWithSocket(connectInfo);
                }
                else
                {
                    disconnectWithSocket(connectInfo);
                }
            }
        }
    }
}
                    
#if THREAD_TYPE == SINGLE_THREAD
#else
void Network::finishProcessingCallback(ConnectInfo* connectInfo)
{
    if((connectInfo->flags & FLAG_DISCONNECTED) != 0)
    {
        if((connectInfo->flags & FLAG_PROCESSING) != 0)
        {
            sendDataToWorkerThread(RECEIVE_TYPE_DISCONNECT, connectInfo);
            connectInfo->flags = 0;
            connectInfo->flags |= FLAG_DISCONNECTED;
        }
        else
        {
            int j;
            for(j = 0; j < listenSocketCount; j++)
            {
                if(listenerInfoVector.at(j)->serverModule == connectInfo->serverModule)
                {
                    if(listenerInfoVector.at(j)->connectInfoMap.erase( connectInfo->fd ) != 1)
                    {
                        ErrorLog("session erase fail");
                    }
                    connectInfoPool.destroy(connectInfo);
                    
                    break;
                }
            }
            
            if(j == listenSocketCount) ErrorLog("not found connectInfo");
        }
    }
    else if(connectInfo->tempDataQueue.empty() == false)
    {
        CharString* tbi = connectInfo->tempDataQueue.front();
        connectInfo->tempDataQueue.pop_front();
        sendDataToWorkerThread(RECEIVE_TYPE_RECEIVE, connectInfo, tbi->string, tbi->stringSize);
        
        delete tbi;
    }
    else
    {
        connectInfo->flags = 0;
    }
}

void Network::sendDataToWorkerThread(int receiveType, ConnectInfo* const _connectInfo, const char* _data, int _dataSize)
{
    DataPacket* dp = new DataPacket();
    dp->receiveType = receiveType;
    dp->connectInfo = _connectInfo;
    dp->dataInfo.deepCopy(_dataSize, _data);
    
    int firstCnt, secondCnt;
    int fitNum;
    
    firstCnt = INT_MAX;
    secondCnt = 0;
    fitNum = 0;
    
    for(int i = 0; i < workerThreadCount; i++)
    {
        secondCnt = workerThreadArray[i]->getDataPacketCount();
        
        if(firstCnt > secondCnt)
        {
            secondCnt = firstCnt;
            fitNum = i;
        }
    }
    
    workerThreadArray[fitNum]->pushDataPacket(dp);
    
    return ;
}

#endif

void Network::sendData(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    const char* pData = data;
    int pDataSize = dataSize;
    while(true)
    {
#if OS_PLATFORM == PLATFORM_LINUX
        ssize_t sendCnt = send(connectInfo->fd, (void*)pData, pDataSize, MSG_NOSIGNAL);
#elif OS_PLATFORM == PLATFORM_MAC
        ssize_t sendCnt = send(connectInfo->fd, (void*)pData, pDataSize, SO_NOSIGPIPE);
#endif
        
        if(sendCnt == pDataSize)
        {
            return ;
        }
        else if(sendCnt < pDataSize)
        {
            pData += sendCnt;
            pDataSize -= sendCnt;
            continue;
        }
        else if(sendCnt < 0)
        {
            DebugLog("ERRNO = %d", errno);
            
            if(errno == EPIPE)
            {
                disconnectWithSocket(connectInfo);
            }
            else if(errno == EAGAIN)
            {
                continue;
            }
            else if(errno == ECONNRESET)
            {
                disconnectWithSocket(connectInfo);
            }
            else
            {
                disconnectWithSocket(connectInfo);
            }
            
            return ;
        }
    }
}

#if THREAD_TYPE == SINGLE_THREAD
void Network::sendPacket(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DataHeader dataHeader;
    dataHeader.dataType = DATA_TYPE_REQ;
    dataHeader.dataSize = dataSize;
    char sendDataArray[dataSize + sizeof(dataHeader)];
    memcpy(sendDataArray, &dataHeader, sizeof(dataHeader));
    memcpy(sendDataArray + sizeof(dataHeader), data, dataSize);
    
    sendData(connectInfo, sendDataArray, sizeof(dataHeader) + dataSize);
    
}
#else
                
void Network::sendPacket(int writePipe, const ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DataHeader dataHeader;
    dataHeader.dataType = DATA_TYPE_REQ;
    dataHeader.dataSize = dataSize;
    char sendDataArray[dataSize + sizeof(dataHeader)];
    memcpy(sendDataArray, &dataHeader, sizeof(dataHeader));
    memcpy(sendDataArray + sizeof(dataHeader), data, dataSize);
    
    sendData(connectInfo, sendDataArray, sizeof(dataHeader) + dataSize);

    finishProcessing(writePipe, connectInfo);
}

void Network::finishProcessing(int threadId, const ConnectInfo* connectInfo)
{
    if(write(threadId, (void*)&connectInfo, sizeof(connectInfo)) < 0)
    {
        ErrorLog("?? errno - %d", errno);
    }
}
#endif

void Network::pingCheck(ListenerInfo* listenerInfo)
{
    long currentTime = getCustomCurrentTime();
    
    dataType_t dataTypeToSend = DATA_TYPE_PING_NOTIFY;
    
    boost::unordered_map<int, ConnectInfo*>::iterator itr;
    
    int disconnectPingCount = listenerInfo->disconnectPingCount;
    
    for (itr = listenerInfo->connectInfoMap.begin(); itr != listenerInfo->connectInfoMap.end(); itr++)
    {
        ConnectInfo* connectInfo = itr->second;
        
#if THREAD_TYPE == SINGLE_THREAD
        if(connectInfo->sendPingCount >= disconnectPingCount)
        {
            
            
#if OS_PLATFORM == PLATFORM_LINUX
            
            epoll_ctl(eventFd, EPOLL_CTL_DEL, connectInfo->fd, event);
            
#elif OS_PLATFORM == PLATFORM_MAC
            
            EV_SET(&connectEvent, connectInfo->fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            
            if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
            {
                ErrorLog("kevent init error");
            }
            
#endif

            
            if(listenerInfo->connectInfoMap.erase( connectInfo->fd ) != 1)
            {
                ErrorLog("session erase fail");
            }
            connectInfoPool.destroy(connectInfo);
        }
        else
        {
            sendData(connectInfo, (char*)&dataTypeToSend, sizeof(dataTypeToSend));
        }

#else
        if ((connectInfo->flags & FLAG_DISCONNECTED) == 0)
        {
            if(connectInfo->sendPingCount >= disconnectPingCount)
            {

                
#if OS_PLATFORM == PLATFORM_LINUX
                
                epoll_ctl(eventFd, EPOLL_CTL_DEL, connectInfo->fd, event);
                
#elif OS_PLATFORM == PLATFORM_MAC
                
                EV_SET(&connectEvent, connectInfo->fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                
                if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
                {
                    ErrorLog("kevent init error");
                }
                
#endif
                
                
                if((connectInfo->flags & FLAG_PROCESSING) != 0)
                {
                    connectInfo->flags |= FLAG_DISCONNECTED;
                }
                else
                {
                    if(listenerInfo->connectInfoMap.erase( connectInfo->fd ) != 1)
                    {
                        ErrorLog("session erase fail");
                    }
                    connectInfoPool.destroy(connectInfo);
                }
            }
            else
            {
                DebugLog("send ping");
                sendData(connectInfo, (char*)&dataTypeToSend, sizeof(dataTypeToSend));
            }
        }
#endif
    }
    
    listenerInfo->lastPingCheckTime = currentTime;
}


int Network::CreateTCPServerSocket(const char* ip, unsigned short port)
{
	int sock;
	struct sockaddr_in servaddr;

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		ErrorLog("socket() failed.\n");
		return -1;
	}

	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) < 0)
	{
		ErrorLog("Bind() failed.\n");
		return -2;
	}

	if (listen(sock, SOMAXCONN)) {
		ErrorLog("Listen() failed.\n");
		return -3;
	}

	return sock;
}

int Network::CreateTCPClientSocket(const char* ip, unsigned short port)
{
    int sock;
    struct sockaddr_in addr;
    
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        ErrorLog("socket() failed.\n");
        return -1;
    }
    
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        ErrorLog("can't connect.\n");
        return -2;
    }
    
    return sock;
}

                
void Network::disconnectWithSocket(ConnectInfo* connectInfo)
{
    int fd = connectInfo->fd;
#if OS_PLATFORM == PLATFORM_LINUX
    
    epoll_ctl(eventFd, EPOLL_CTL_DEL, fd, event);
    
#elif OS_PLATFORM == PLATFORM_MAC
    
    EV_SET(&connectEvent, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    
    
    if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
    {
        ErrorLog("kevent init error");
    }
    
#endif
    
    close(fd);
    
#if THREAD_TYPE == SINGLE_THREAD
    workerThreadArray[0]->disconnected(connectInfo);
    
    int j;
    for(j = 0; j < listenSocketCount; j++)
    {
        if(listenerInfoVector.at(j)->serverModule == connectInfo->serverModule)
        {
            if(listenerInfoVector.at(j)->connectInfoMap.erase( connectInfo->fd ) != 1)
            {
                ErrorLog("session erase fail");
            }
            connectInfoPool.destroy(connectInfo);
            
            break;
        }
    }
    
    if(j == listenSocketCount) ErrorLog("not found connectInfo");
#else
    connectInfo->flags |= FLAG_DISCONNECTED;
    
    if((connectInfo->flags & FLAG_PROCESSING) != 0)
    {
        
    }
    else
    {
        sendDataToWorkerThread(RECEIVE_TYPE_DISCONNECT, connectInfo);
    }
#endif
    
}