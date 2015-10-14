#ifndef __LOBBY_RECEIVE_HANDLER_H__
#define __LOBBY_RECEIVE_HANDLER_H__

#include "Network.h"
#include "BasicPacket.h"

class LobbyReceiveHandler
{
public:
	LobbyReceiveHandler();
	~LobbyReceiveHandler();
	bool initialize();

	void receiveData(const ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
	void sessionIn(const ConnectInfo* connectInfo);
    void sessionOut(const ConnectInfo* connectInfo);

	void HandleFirstConnectReq(const ConnectInfo* connectInfo, const char* body, const int bodySize);

private:
	char m_sendBuffer[5000];
};

#endif //__LOBBY_RECEIVE_HANDLER_H__
