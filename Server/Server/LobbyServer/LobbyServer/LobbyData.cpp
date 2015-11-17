

//
// FrontData.cpp
//
// create by kimyc on 2014.4.7..
//

#include "LobbyData.h"
#include "Network.h"
#include "LobbyDefine.h"
#include "tinyxml2.h"
#include "Log.h"

#include <string.h> // strcmp header

#if OS_PLATFORM == PLATFORM_LINUX
const char* LOG_NAME = "./connect_info.xml";
#else
const char* LOG_NAME = "/Users/kimyongchan/MobileStar/Server/Server/LobbyServer/LobbyServer/connect_info.xml";
#endif

////////////////FrontData

using namespace tinyxml2;

LobbyData::LobbyData()
{
	networkInfoList = NULL;
    networkInfoCount = 0;
}

LobbyData::~LobbyData()
{
	delete networkInfoList;
}


bool LobbyData::Initialize()
{
	if (LoadNetworkInfo() == false) return false;

	return true;
}

bool LobbyData::LoadNetworkInfo()
{
    tinyxml2::XMLDocument doc;
    
    if (doc.LoadFile(LOG_NAME) != 0)
    {
        ErrorLog("LoadFile : connect_info.xml");
        return false;
    }
    
    XMLElement* rootNode = doc.FirstChildElement();
    
    
    NetworkInfo tempNetworkInfoList[10];
    
    for (XMLElement* element = rootNode->FirstChildElement(); element; element = element->NextSiblingElement())
    {
        int8_t module = 0;
        const char* serverModule = element->FirstChildElement("server_module")->GetText();
        if (strcmp(serverModule, "cache_server") == 0)
        {
            module = SERVER_MODULE_CACHE_SERVER;
        }
        else if (strcmp(serverModule, "front_server") == 0)
        {
            module = SERVER_MODULE_FRONT_SERVER;
        }
        else if (strcmp(serverModule, "lobby_server") == 0)
        {
            module = SERVER_MODULE_LOBBY_SERVER;
        }
        else if (strcmp(serverModule, "game_server") == 0)
        {
            module = SERVER_MODULE_GAME_SERVER;
        }
        else if (strcmp(serverModule, "chatting_server") == 0)
        {
            module = SERVER_MODULE_CHATTING_SERVER;
        }
        else if (strcmp(serverModule, "client") == 0)
        {
            module = SERVER_MODULE_CLIENT;
        }
        else
        {
            ErrorLog("serverModule is undefined - %s", serverModule);
            return false;
        }
        
        int8_t type = 0;
        const char* serverType = element->FirstChildElement("server_type")->GetText();
        if (strcmp(serverType, "server") == 0)
        {
            type = SERVER_TYPE_SERVER;
        }
        else if (strcmp(serverType, "sible") == 0)
        {
            type = SERVER_TYPE_SIBLE;
        }
        else if (strcmp(serverType, "client") == 0)
        {
            type = SERVER_TYPE_CLIENT;
        }
        else
        {
            ErrorLog("serverType is undefined - %s", serverType);
            return false;
        }
        
        //		const char* name = element->FirstChildElement("name")->GetText();
        const char* ip = element->FirstChildElement("ip")->GetText();
        
        int port = 0;
        element->FirstChildElement("port")->QueryIntText(&port);
        int pingInterval = 0;
        element->FirstChildElement("ping_interval")->QueryIntText(&pingInterval);
        int disconnetCount = 0;
        element->FirstChildElement("disconnect_count")->QueryIntText(&disconnetCount);
        
        tempNetworkInfoList[networkInfoCount].module = module;
        tempNetworkInfoList[networkInfoCount].type = type;
        
        if (sprintf(tempNetworkInfoList[networkInfoCount].ip, "%s", ip) < 0)
        {
            ErrorLog("ipAddress copy fail");
            return false;
        }
        tempNetworkInfoList[networkInfoCount].port = (int16_t)port;
        tempNetworkInfoList[networkInfoCount].sendPingInterval = pingInterval;
        tempNetworkInfoList[networkInfoCount].disconnectPingCount = disconnetCount;
        
        networkInfoCount++;
        
        DebugLog("type = %s role = %s ip = %s port = %d %d %d", serverModule, serverType, ip, port, pingInterval, disconnetCount);
    }
    
    networkInfoList = new NetworkInfo[networkInfoCount];
    memcpy(networkInfoList, tempNetworkInfoList, sizeof(NetworkInfo) * networkInfoCount);
    
    return true;
}




const NetworkInfo* LobbyData::getNetworkInfo(int serverModule)
{
    for(int i = 0; i <networkInfoCount; i++)
    {
        if(networkInfoList[i].module == serverModule)
        {
            return &networkInfoList[i];
        }
    }
    
    return NULL;
}





