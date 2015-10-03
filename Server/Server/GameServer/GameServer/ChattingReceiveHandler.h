#ifndef __CHATTING_RECEIVE_HANDLER_H__
#define __CHATTING_RECEIVE_HANDLER_H__

#include "Network.h"
#include "BasicPacket.h"

class ChattingReceiveHandler
{
public:
	ChattingReceiveHandler();
	~ChattingReceiveHandler();
	bool initialize();

	void receiveData(const ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
	void sessionIn(const ConnectInfo* connectInfo);
    void sessionOut(const ConnectInfo* connectInfo);

//	void handleFirstConnectReq(const ConnectInfo* connectInfo, const char* body, const int bodySize);
//	void handleChattingReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void handleGetChannelListReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void handleMoveChannelReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void handleGetUserListReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void handleGetUserInfoReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void handleGetFriendListReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void handleAddResponseFriendReq(const ConnectInfo* connectInfo, const char* body, int bodySize);
//	void HandleEnterFrontUserReq(Session* const session, const char* body, int bodySize);

private:
	char m_sendBuffer[5000];
};

#endif //__CHATTING_RECEIVE_HANDLER_H__
