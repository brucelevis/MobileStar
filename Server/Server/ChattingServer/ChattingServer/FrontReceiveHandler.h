#ifndef __FRONT_RECEIVE_HANDLER_H__
#define __FRONT_RECEIVE_HANDLER_H__

#include "Network.h"

class FrontReceiveHandler
{
public:
	FrontReceiveHandler();
	~FrontReceiveHandler();
	bool initialize();

	virtual void receiveData(const ConnectInfo* connectInfo, const uint32_t cmd, const char* body, const int bodySize);
	virtual void sessionIn(const ConnectInfo* connectInfo);
	virtual void sessionOut(const ConnectInfo* connectInfo);

//	void handleFirstConnectRes(const ConnectInfo* connectInfo, const char* body, const int bodySize);
//	void handleEnterFrontUserReq(const ConnectInfo* connectInfo, const char* body, int bodySize);

private:
	char m_sendBuffer[5000];
};

#endif //__FRONT_RECEIVE_HANDLER_H__
