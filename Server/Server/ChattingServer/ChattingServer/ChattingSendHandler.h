#ifndef __CHATTING_SEND_HANDLER_H__
#define __CHATTING_SEND_HANDLER_H__

#include "Network.h"

class ChattingSendHandler
{
public:
	ChattingSendHandler();
	~ChattingSendHandler();
	bool initialize();
	
//	void handleFirstConnectRes(const ConnectInfo* connectInfo, Channel* channel, int16_t requestInfoCount, RequestInfo* requestInfoList);
//	void handleChattingNotify(Channel* channel, const char* nickName, int8_t chattingLen, const char* chatting);
//	void handleGetChannelListRes(const ConnectInfo* connectInfo, int8_t channelCount, ChannelInfo* channelInfo);
//	void handleMoveChannelRes(const ConnectInfo* connectInfo, int16_t channelNo, const char* channelName);
//	void handleGetUserListRes(const ConnectInfo* connectInfo, Channel* channel);
//	void handleGetUserInfoRes(const ConnectInfo* connectInfo, UserInfo* userInfo);
//	void handleGetFriendListRes(const ConnectInfo* connectInfo, int16_t nickNameInfoListCount, NickNameInfo* nickNameInfoList, int8_t* online);
//	void handleAddRequestFriendRes(const ConnectInfo* connectInfo);
//	void handleAddRequestFriendNotify(const ConnectInfo* connectInfo, int8_t nickNameLen, const char* nickName);
//	void handleAddResponseFriendRes(const ConnectInfo* connectInfo);
//	void handleRemoveFriendRes(const ConnectInfo* connectInfo);
//
//	void handleFailPacket(const ConnectInfo* connectInfo, Cmd_t cmd, int32_t failReason);

private:

	char m_sendBuffer[5000];
};

#endif //__CHATTING_SEND_HANDLER_H__
