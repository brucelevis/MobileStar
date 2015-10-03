#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

#include <pthread.h>
#include <deque>

class ConnectInfo;
class DataPacket;

struct ThreadPipe
{
    int readPipe;
    int writePipe;
};

class WorkerThread
{
public:
	WorkerThread();
	virtual ~WorkerThread();
	bool initialize();
	void run();

    void pushDataPacket(DataPacket* dataPacket);
    DataPacket* popDataPacket();
    
    virtual void connected(ConnectInfo* connectInfo) = 0;
    virtual void disconnected(ConnectInfo* connectInfo) = 0;
    virtual void receiveData(ConnectInfo* connectInfo, const char* data, int dataSize) = 0;
    
    pthread_t* getTid() { return &tid; }
    pthread_cond_t* getCond() { return &cond; }
    int getDataPacketCount();
    
    ThreadPipe threadPipe;

protected:
	void lock() { pthread_mutex_lock(&mutex); }
	void unLock() { pthread_mutex_unlock(&mutex); }
    void unLockAndWait() { pthread_cond_wait(&cond, &mutex); pthread_mutex_unlock(&mutex); }

protected:
    pthread_t tid;
	pthread_mutex_t mutex;
    pthread_cond_t cond;
    std::deque<DataPacket*>* dataPacketQueue;
};

#endif //__WORKER_THREAD_H__