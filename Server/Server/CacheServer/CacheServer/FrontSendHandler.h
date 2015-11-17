#ifndef __FRONT_SEND_HANDLER_H__
#define __FRONT_SEND_HANDLER_H__

#include "Network.h"

class FrontSendHandler
{
public:
	FrontSendHandler();
	~FrontSendHandler();
	bool initialize();

//	void HandleFirstConnectReq(const ConnectInfo* connectInfo);
//	void HandleEnterFrontUserRes(const ConnectInfo* connectInfo, SessionId_t sid);
//	void HandleFailPacket(const ConnectInfo* connectInfo, SessionId_t sid, Cmd_t cmd, int32_t failReason);

private:
	char m_sendBuffer[5000];
};

#endif //__FRONT_SEND_HANDLER_H__
