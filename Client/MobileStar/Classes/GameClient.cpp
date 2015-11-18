
//
// GameClient.cpp
//
// create by kimyc on 2014.4.7..
//

#include "GameClient.h"
#include <string.h>

GameClient::GameClient()
{
    memset(&sessionId, 0, sizeof(sessionId));
}

GameClient& GameClient::GetInstance()
{
	static GameClient Instance;
	return Instance;
}

GameClient* GameClient::GetPointer()
{
	return &GetInstance();
}

const GameClient& GameClient::GetConst()
{
	return GetInstance();
}


void GameClient::intToString(std::string& str, int integer)
{
	if (integer == 0)
		str = "0";
	else
	{
		std::string temp = "";
		while (integer>0)
		{
			temp += integer % 10 + 48;
			integer /= 10;
		}
		for (int i = 0; i<temp.length(); i++)
			str += temp[temp.length() - i - 1];
	}
}


GameClient::~GameClient()
{
    
}


bool GameClient::Initialize()
{
	userInfo = new UserInfo();
	memset(userInfo, 0, sizeof(UserInfo));

	currentScene = NO_SCENE_NOW;
	channelNo = -1;

    popUp = new CustomPopUp();
    
    isConnectedWithChattingServer = false;
    isConnectedWithLobbyServer = false;
	return true;
}

void GameClient::SetUserInfo(UserInfo* _userInfo)
{
	memcpy(userInfo, _userInfo, sizeof(UserInfo));
}