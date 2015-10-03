
//
// FrontData.h
//
// create by kimyc on 2014.6.28
//


#ifndef __FRONT_DATA_H__
#define __FRONT_DATA_H__

#include "BasicPacket.h"
#include "BasicDefines.h"

class NetworkInfo;

class FrontData
{
public:
	FrontData();
	~FrontData();
	bool Initialize();
	bool LoadNetworkInfo();

    int networkInfoCount;
	NetworkInfo* networkInfoList;
};

#endif //__LOBBY_DATA_H__


