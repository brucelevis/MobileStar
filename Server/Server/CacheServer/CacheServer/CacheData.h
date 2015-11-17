
//
// FrontData.h
//
// create by kimyc on 2014.6.28
//


#ifndef __CACHE_DATA_H__
#define __CACHE_DATA_H__

#include "BasicPacket.h"
#include "BasicDefines.h"

class NetworkInfo;

class CacheData
{
public:
	CacheData();
	~CacheData();
	bool Initialize();
	bool LoadNetworkInfo();

    const NetworkInfo* getNetworkInfo(int serverModule);
    
    int networkInfoCount;
	NetworkInfo* networkInfoList;
};

#endif //__CACHE_DATA_H__


