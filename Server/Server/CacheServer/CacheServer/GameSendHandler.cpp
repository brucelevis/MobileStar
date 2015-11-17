#include "GameSendHandler.h"
#include "Log.h"

GameSendHandler::GameSendHandler()
{

}

GameSendHandler::~GameSendHandler()
{

}

bool GameSendHandler::initialize()
{

	return true;
}

void GameSendHandler::sendFailPacket(const ConnectInfo* connectInfo, command_t cmd, int32_t failReason)
{
//	GameLobbyPacket::FailPacket packet;
//	memset(&packet, 0, sizeof(packet));
//
//	packet.failReason = failReason;
//
//	memcpy(m_sendBuffer, &packet, sizeof(packet));

//	LobbyServer::getInstance()->network->sendData(GAME_SERVER, session, cmd, m_sendBuffer, sizeof(packet));
}