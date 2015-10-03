
//
// FrontData.h
//
// create by kimyc on 2014.6.28
//


#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include "BasicPacket.h"
#include "BasicDefines.h"

class NetworkInfo;

/*
struct ConnectXmlInfo
{
	int8_t	serverModule;
	int8_t	serverType;
	int16_t	port;
	char	ipAddress[BasicPacket::MAX_IP_ADDRESS_LEN + 1];
};*/

class GameData
{
public:
	GameData();
	~GameData();
	bool Initialize();
	bool LoadNetworkInfo();

    const NetworkInfo* getNetworkInfo(int serverModule);

    
    int networkInfoCount;
	NetworkInfo* networkInfoList;
};

#endif //__GAME_DATA_H__


