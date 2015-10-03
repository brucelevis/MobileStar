#include "WorkerThread.h"
#include "Network.h"
#include "Log.h"
#include <stdlib.h>

WorkerThread::WorkerThread()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}


WorkerThread::~WorkerThread()
{
    lock();
    
    if (dataPacketQueue->empty() == false) //delete data packet
    {
        DataPacket* dp = NULL;
        std::deque<DataPacket*>::iterator itr;
        for(itr = dataPacketQueue->begin(); itr != dataPacketQueue->end(); itr++)
        {
            dp = dataPacketQueue->front();
            dataPacketQueue->pop_front();
            delete dp;
        }
    }
    
    unLock();
    
	pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

bool WorkerThread::initialize()
{
    dataPacketQueue = new std::deque<DataPacket*>();
    dataPacketQueue->clear();
    
	return true;
}


void WorkerThread::run()
{
	while (true)
	{
		DataPacket* dp = popDataPacket();
		if (dp != NULL)
		{
            if(dp->receiveType == RECEIVE_TYPE_RECEIVE)
            {
                receiveData(dp->connectInfo, dp->dataInfo.string, dp->dataInfo.stringSize);
            }
            else if(dp->receiveType == RECEIVE_TYPE_CONNECT)
            {
                connected(dp->connectInfo);
            }
            else if(dp->receiveType == RECEIVE_TYPE_DISCONNECT)
            {
                disconnected(dp->connectInfo);
            }
            else
            {
                ErrorLog("invalid receive type - %d", dp->receiveType);
            }
            delete dp;
		}
	}
}


void WorkerThread::pushDataPacket(DataPacket* dataPacket)
{
    lock();
    
    dataPacketQueue->push_back(dataPacket);
    
    if(dataPacketQueue->size() == 1)
        pthread_cond_signal(&cond);
    
    unLock();
}


DataPacket* WorkerThread::popDataPacket()
{
    lock();
    
    DataPacket* dp = NULL;
    if (dataPacketQueue->empty() == false)
    {
        dp = dataPacketQueue->front();
        dataPacketQueue->pop_front();
        unLock();
    }
    else
    {
        unLockAndWait();
    }
    
    return dp;
}


int WorkerThread::getDataPacketCount()
{
//    lock();
//    int cnt = (int)dataPacketQueue->size();
//    unLock();
//    return cnt;
    
    // not use lock because this function doesn't want correct value
    
    return (int)dataPacketQueue->size();
    
}
