
//
// GameClient.h
//
// create by kimyc on 2014.6.28
//

#ifndef __GAME_CLIENT_H__
#define __GAME_CLIENT_H__

#include "GameDefines.h"
#include "BasicPacket.h"
#include "CustomPopUp.h"
#include <string>
#include <vector>

class GameClient
{
public:
	static GameClient& GetInstance();
	static GameClient* GetPointer();
	static const GameClient& GetConst();
	

public:
	static void intToString(std::string& str, int integer);
private:
	GameClient();
	~GameClient();

public:
	bool Initialize();
	void SetUserInfo(UserInfo* _userInfo);
	UserInfo* GetUserInfo() {
		return userInfo;
	}

public:
    CustomPopUp* popUp;
    
    SessionId_t sessionId;
	UserInfo* userInfo;
	int currentScene;
	int16_t channelNo;

    int roomNo;
    int tribe;
    
    int myGameIndex;
    int8_t userCount;
    GameUserInfo gameUserInfo[MAX_ROOM_SLOT_COUNT];
    
    bool isConnectedWithChattingServer;
    bool isConnectedWithLobbyServer;
    
    char gameServerIp[MAX_IP_ADDRESS_LEN];
    uint16_t gameServerPort;
};

#endif //__GAME_CLIENT_H__


