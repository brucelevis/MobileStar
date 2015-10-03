#include "GameDefine.h"
#include "FrontSendHandler.h"
//#include "FrontLobbyPacket.h"
#include "GameServer.h"
#include "Log.h"
#include "GameData.h"

FrontSendHandler::FrontSendHandler()
{

}

FrontSendHandler::~FrontSendHandler()
{

}

bool FrontSendHandler::initialize()
{

	return true;
}
/*
void FrontSendHandler::HandleFirstConnectReq(Session* const session)
{
	DebugLog("FrontSendHandler::HandleFirstConnectReq");

	FrontLobbyPacket::FirstConnectReqPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.packetVersion = FrontLobbyPacket::PACKET_VERSION;
	memcpy(packet.clientIp, LobbyServer::GetInstance().lobbyData->connectInfo[CLIENT].ipAddress, BasicPacket::MAX_IP_ADDRESS_LEN);
	packet.clientPort = LobbyServer::GetInstance().lobbyData->connectInfo[CLIENT].port;

	
	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(FRONT_SERVER, _session, FrontLobbyPacket::FIRST_CONNECT_REQ, m_sendBuffer, sizeof(packet));
}

void FrontSendHandler::HandleEnterFrontUserRes(Session* const session, SessionId_t sid)
{
	DebugLog("FrontSendHandler::HandleEnterFrontUserRes");

	FrontLobbyPacket::EnterFrontUserResPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.sid = sid;

	memcpy(m_sendBuffer, &packet, sizeof(packet));

	LobbyServer::GetInstance().network->SendData(FRONT_SERVER, session, FrontLobbyPacket::ENTER_FRONT_USER_RES, m_sendBuffer, sizeof(packet));
}

void FrontSendHandler::HandleFailPacket(Session* session, SessionId_t sid, Cmd_t cmd, int32_t failReason)
{

}*/