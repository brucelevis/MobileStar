#include "GameDefine.h"
#include "LobbySendHandler.h"
#include "ClientLobbyPacket.h"
#include "GameServer.h"
#include "Log.h"
#include "GameData.h"
//#include "Channel.h"
#include "User.h"

LobbySendHandler::LobbySendHandler()
{

}

LobbySendHandler::~LobbySendHandler()
{

}

bool LobbySendHandler::initialize()
{

	return true;
}
/*
void LobbySendHandler::HandleFirstConnectRes(const ConnectInfo* connectInfo)
{
	DebugLog("LobbySendHandler::HandleFirstConnectRes");

	ClientLobbyPacket::FirstConnectResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.channelNo = channel->GetChannelNo();
	memcpy(packet.channelName, channel->GetChannelName(), BasicPacket::MAX_CHANNEL_NAME_LEN);

	for (int i = 0; i < requestInfoCount; i++)
	{
		packet.requestInfoList[i].requestType = requestInfoList[i].requestType;
		packet.requestInfoList[i].nickNameInfo.nickNameLen = requestInfoList[i].nickNameInfo.nickNameLen;
		memcpy(packet.requestInfoList[i].nickNameInfo.nickName, requestInfoList[i].nickNameInfo.nickName, BasicPacket::MAX_NICK_NAME_LEN);
	}

	packet.requestInfoCount = requestInfoCount;
	int packetSize = sizeof(packet) - (sizeof(BasicPacket::RequestInfo)*(BasicPacket::MAX_REQUEST_INFO_COUNT - requestInfoCount));
	if (packetSize < 0)
	{
		ErrorLog("packetSize %d", packetSize);
		return;
	}

	memset(m_sendBuffer, 0, sizeof(SEND_BUF_SIZE));
	memcpy(m_sendBuffer, &packet, packetSize);

	LobbyServer::GetInstance().network->SendData(CLIENT, session, ClientLobbyPacket::FIRST_CONNECT_RES, m_sendBuffer, packetSize);
}


*/