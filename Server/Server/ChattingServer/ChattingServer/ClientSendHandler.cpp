#include "ClientSendHandler.h"
#include "Log.h"
#include "User.h"

#include "ClientLobbyPacket.h"

ClientSendHandler::ClientSendHandler()
{

}

ClientSendHandler::~ClientSendHandler()
{

}

bool ClientSendHandler::initialize()
{

	return true;
}

void ClientSendHandler::handleFirstConnectRes(ConnectInfo* connectInfo, Channel* channel, int16_t requestInfoCount, RequestInfo* requestInfoList)
{
	DebugLog("ClientSendHandler::HandleFirstConnectRes");
/*
	ClientLobbyPacket::FirstConnectResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.channelNo = channel->getChannelNo();
	memcpy(packet.channelName, channel->getChannelName(), MAX_CHANNEL_NAME_LEN);

	for (int i = 0; i < requestInfoCount; i++)
	{
		packet.requestInfoList[i].requestType = requestInfoList[i].requestType;
		packet.requestInfoList[i].nickNameInfo.nickNameLen = requestInfoList[i].nickNameInfo.nickNameLen;
		memcpy(packet.requestInfoList[i].nickNameInfo.nickName, requestInfoList[i].nickNameInfo.nickName, MAX_NICK_NAME_LEN);
	}

	packet.requestInfoCount = requestInfoCount;
	int packetSize = sizeof(packet) - (sizeof(RequestInfo)*(MAX_REQUEST_INFO_COUNT - requestInfoCount));
	if (packetSize < 0)
	{
		ErrorLog("packetSize %d", packetSize);
		return;
	}

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, packetSize);

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::FIRST_CONNECT_RES, m_sendBuffer, packetSize);
*/}
/*
void ClientSendHandler::HandleChattingNotify(Channel* channel, const char* nickName, int8_t chattingLen, const char* chatting)
{
	DebugLog("ClientSendHandler::HandleChattingNotify");

	ClientLobbyPacket::ChattingNotifyPacket packet;
	memset(&packet, 0, sizeof(packet));

	memcpy(packet.nickName, nickName, MAX_NICK_NAME_LEN);
	packet.chattingLen = chattingLen;
	memcpy(packet.chatting, chatting, MAX_CHATTING_LEN);

	int packetSize = sizeof(packet) - (MAX_CHATTING_LEN - chattingLen + 1);
	if (packetSize < 0)
	{
		ErrorLog("packetSize %d", packetSize);
		return;
	}

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, packetSize);

	const std::list<User*> userList = channel->GetUserList();
	std::list<User*>::const_iterator iter = userList.begin();
	DebugLog("dzdz");
	User* user;
	for (iter = userList.begin(); iter != userList.end(); iter++)
	{
		user = *iter;
		if (user->GetSession() == NULL)
		{
			DebugLog("user not login lobby %s", user->GetNickName());
			continue;
		}

		LobbyServer::GetInstance().network->SendData(CLIENT, user->GetSession(), ClientLobbyPacket::CHATTING_NOTIFY, m_sendBuffer, packetSize);
	}
}

void ClientSendHandler::HandleGetChannelListRes(Session* const session, int8_t channelCount, ChannelInfo* channelInfo)
{
	DebugLog("ClientSendHandler::HandleGetChannelListRes");

	ClientLobbyPacket::GetChannelListResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.channelCount = channelCount;

	for (int i = 0; i < channelCount; i++)
	{
		packet.channelInfo[i].channelNo = channelInfo[i].channelNo;
		memcpy(packet.channelInfo[i].channelName, channelInfo[i].channelName, MAX_CHANNEL_NAME_LEN);
	}

	int packetSize = sizeof(packet) - ((MAX_CHANNEL_COUNT - packet.channelCount) * sizeof(ChannelInfo));

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, packetSize);

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::GET_CHANNEL_LIST_RES, m_sendBuffer, packetSize);
}

void ClientSendHandler::HandleMoveChannelRes(Session* const session, int16_t channelNo, const char* channelName)
{
	DebugLog("ClientSendHandler::HandleMoveChannelRes");

	ClientLobbyPacket::MoveChannelResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.channelInfo.channelNo = channelNo;
	memcpy(packet.channelInfo.channelName, channelName, MAX_CHANNEL_NAME_LEN);

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::MOVE_CHANNEL_RES, m_sendBuffer, sizeof(packet));

	DebugLog("%d %s", packet.channelInfo.channelNo, packet.channelInfo.channelName);

}

void ClientSendHandler::HandleGetUserListRes(Session* const session, Channel* channel)
{
	DebugLog("ClientSendHandler::HandleGetUserListRes");

	ClientLobbyPacket::GetUserListResPacket packet;
	memset(&packet, 0, sizeof(packet));

	const std::list<User*> userList = channel->GetUserList();
	std::list<User*>::const_iterator iter = userList.begin();

	int16_t userCount;
	User* user;
	for (iter = userList.begin(), userCount = 0; iter != userList.end(); iter++)
	{
		user = *iter;
		if (user->GetSession() == NULL)
		{
			continue;
		}

		memcpy(packet.nickNameInfoList[userCount].nickName, user->GetNickName(), MAX_NICK_NAME_LEN);
		userCount++;
	}

	packet.nickNameInfoListCount = userCount;
	int packetSize = sizeof(packet) - (sizeof(NickNameInfo)*(MAX_USER_COUNT_IN_CHANNEL - userCount));
	if (packetSize < 0)
	{
		ErrorLog("packetSize %d", packetSize);
		return;
	}

	DebugLog("packetSize %d userCount %d", packetSize, userCount);

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, packetSize);

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::GET_USER_LIST_RES, m_sendBuffer, packetSize);
}

void ClientSendHandler::HandleGetUserInfoRes(Session* const session, UserInfo* userInfo)
{
	DebugLog("ClientSendHandler::HandleGetUserInfoRes");

	ClientLobbyPacket::GetUserInfoResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.nickNameLen = userInfo->nickNameLen;
	memcpy(packet.nickName, userInfo->nickName, userInfo->nickNameLen);
	packet.gold = userInfo->gold;
	packet.memoLen = userInfo->memoLen;
	memcpy(packet.memo, userInfo->memo, userInfo->memoLen);
	packet.commonWin = userInfo->commonWin;
	packet.commonLose = userInfo->commonLose;
	packet.commonDiss = userInfo->commonDiss;
	packet.rankWin = userInfo->rankWin;
	packet.rankLose = userInfo->rankLose;
	packet.rankDiss = userInfo->rankDiss;
	packet.grade = userInfo->grade;
	packet.gradeReachedCount = userInfo->gradeReachedCount;
	packet.point = userInfo->point;
	packet.icon = userInfo->icon;

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::GET_USER_INFO_RES, m_sendBuffer, sizeof(packet));

//	DebugLog("%d %s", packet.channelInfo.channelNo, packet.channelInfo.channelName);


}

void ClientSendHandler::HandleGetFriendListRes(Session* const session, int16_t nickNameInfoListCount, NickNameInfo* nickNameInfoList, int8_t* online)
{
	DebugLog("ClientSendHandler::HandleGetFriendListRes");

	ClientLobbyPacket::GetFriendListResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.nickNameInfoWithOnlineListCount = nickNameInfoListCount;

	for (int i = 0; i < nickNameInfoListCount; i++)
	{
		packet.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen = nickNameInfoList[i].nickNameLen;
		memcpy(packet.nickNameInfoWithOnlineList[i].nickNameInfo.nickName, nickNameInfoList[i].nickName, MAX_NICK_NAME_LEN);
		packet.nickNameInfoWithOnlineList[i].online = online[i];
	}

	int packetSize = sizeof(packet) - (sizeof(NickNameInfoWithOnline)*(MAX_FRIEND_COUNT - nickNameInfoListCount));
	if (packetSize < 0)
	{
		ErrorLog("packetSize %d", packetSize);
		return;
	}

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, packetSize);

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::GET_FRIEND_LIST_RES, m_sendBuffer, packetSize);

}

void ClientSendHandler::HandleAddRequestFriendRes(Session* const session)
{
	DebugLog("ClientSendHandler::HandleAddRequestFriendRes");

	ClientLobbyPacket::AddRequestFriendResPacket packet;
	memset(&packet, 0, sizeof(packet));

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::ADD_REQUEST_FRIEND_RES, m_sendBuffer, sizeof(packet));
}

void ClientSendHandler::HandleAddRequestFriendNotify(Session* const session, int8_t nickNameLen, const char* nickName)
{
	DebugLog("ClientSendHandler::HandleAddRequestFriendNotify");

	ClientLobbyPacket::AddRequestFriendNotifyPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.nickNameInfo.nickNameLen = nickNameLen;
	memcpy(packet.nickNameInfo.nickName, nickName, MAX_NICK_NAME_LEN);

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::ADD_REQUEST_FRIEND_NOTIFY, m_sendBuffer, sizeof(packet));

}

void ClientSendHandler::HandleAddResponseFriendRes(Session* const session)
{
	DebugLog("ClientSendHandler::HandleAddResponseFriendRes");

	ClientLobbyPacket::AddResponseFriendResPacket packet;
	memset(&packet, 0, sizeof(packet));

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::ADD_RESPONSE_FRIEND_RES, m_sendBuffer, sizeof(packet));

}

void ClientSendHandler::HandleRemoveFriendRes(Session* const session)
{
	DebugLog("ClientSendHandler::HandleRemoveFriendRes");

	ClientLobbyPacket::RemoveFriendResPacket packet;
	memset(&packet, 0, sizeof(packet));

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::REMOVE_FRIEND_RES, m_sendBuffer, sizeof(packet));
}

void ClientSendHandler::HandleFailPacket(Session* const session, Cmd_t cmd, int32_t failReason)
{
	ClientLobbyPacket::FailPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.failReason = failReason;

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(CLIENT, session, cmd, m_sendBuffer, sizeof(packet));
}
*/