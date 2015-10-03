#ifndef __LOBBY_SEND_HANDLER_H__
#define __LOBBY_SEND_HANDLER_H__

#include "Network.h"

class LobbySendHandler
{
public:
	LobbySendHandler();
	~LobbySendHandler();
	bool initialize();
	
//	void HandleFirstConnectRes(const ConnectInfo* connectInfo);

private:

	char m_sendBuffer[1024];
};

#endif //__LOBBY_SEND_HANDLER_H__
