#include "GameReceiveHandler.h"
#include "Log.h"
#include "GameSendHandler.h"

GameReceiveHandler::GameReceiveHandler()
{

}

GameReceiveHandler::~GameReceiveHandler()
{

}

bool GameReceiveHandler::initialize()
{

	return true;
}

void GameReceiveHandler::sessionIn(const ConnectInfo* connectInfo)
{
	DebugLog("GameReceiveHandler::SessionIn");

}

void GameReceiveHandler::sessionOut(const ConnectInfo* connectInfo)
{
	DebugLog("GameReceiveHandler::SessionOut");

}

void GameReceiveHandler::receiveData(const ConnectInfo* connectInfo, command_t cmd, const char* body, const int bodySize)
{
	switch(cmd)
	{

	default:
		ErrorLog("invalid command - type = %d", cmd);
		return ;
	}

}