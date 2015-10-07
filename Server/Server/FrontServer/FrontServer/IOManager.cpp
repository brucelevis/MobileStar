#include "IOManager.h"
#include "Log.h"
#include "Network.h"

#include "FrontDefine.h"
#include "FrontServer.h"

#include "FrontReceiveHandler.h"
#include "FrontSendHandler.h"
#include "LobbyReceiveHandler.h"
#include "LobbySendHandler.h"
#include "GameReceiveHandler.h"
#include "GameSendHandler.h"
#include "ChattingReceiveHandler.h"
#include "ChattingSendHandler.h"
#include "ClientReceiveHandler.h"
#include "ClientSendHandler.h"

#include "ClientFrontPacket.h"
#include "LobbyFrontPacket.h"
#include "GameFrontPacket.h"
#include "User.h"
#include "ServerInfo.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

IOManager::IOManager(FrontSendHandler* _frontSendHandler, LobbySendHandler* _lobbySendHandler, GameSendHandler* _gameSendHandler, ChattingSendHandler* _chattingSendHandler, ClientSendHandler* _clientSendHandler, FrontReceiveHandler* _frontReceiveHandler, LobbyReceiveHandler* _lobbyReceiveHandler, GameReceiveHandler* _gameReceiveHandler, ChattingReceiveHandler* _chattingReceiveHandler, ClientReceiveHandler* _clientReceiveHandler)
{
    frontReceiveHandler = _frontReceiveHandler;
    frontSendHandler = _frontSendHandler;
    lobbyReceiveHandler = _lobbyReceiveHandler;
    lobbySendHandler = _lobbySendHandler;
    gameReceiveHandler = _gameReceiveHandler;
    gameSendHandler = _gameSendHandler;
    chattingReceiveHandler = _chattingReceiveHandler;
    chattingSendHandler = _chattingSendHandler;
    clientReceiveHandler = _clientReceiveHandler;
    clientSendHandler = _clientSendHandler;
    
    createUserNo = 1;
}


IOManager::~IOManager()
{
    delete frontReceiveHandler;
    delete frontSendHandler;
    delete lobbyReceiveHandler;
    delete lobbySendHandler;
    delete gameReceiveHandler;
    delete gameSendHandler;
    delete clientReceiveHandler;
    delete clientSendHandler;
}

void IOManager::connected(ConnectInfo* connectInfo)
{
    DebugLog("connected");
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_LOGIN_SERVER:
            loginSessionIn(connectInfo);
            break;
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveHandler->sessionIn(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionIn(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameSessionIn(connectInfo);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveHandler->sessionIn(connectInfo);
            break;
        case SERVER_MODULE_CLIENT:
            clientSessionIn(connectInfo);
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            break;
    }
    
#if THREAD_TYPE == SINGLE_THREAD
#else
    DebugLog("writePipe - %d", threadPipe.writePipe);
    FrontServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
#endif
}


void IOManager::disconnected(ConnectInfo* connectInfo)
{
    DebugLog("disconnected");

    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_LOGIN_SERVER:
            loginSessionOut(connectInfo);
            break;
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveHandler->sessionOut(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionOut(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameSessionOut(connectInfo);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveHandler->sessionOut(connectInfo);
            break;
        case SERVER_MODULE_CLIENT:
            clientSessionOut(connectInfo);
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            break;
    }
    
#if THREAD_TYPE == SINGLE_THREAD
#else
    FrontServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
#endif
}


void IOManager::receiveData(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DebugLog("reciveData");
    
    if(connectInfo->serverModule == SERVER_MODULE_LOGIN_SERVER)
    {
        loginReceiveData(connectInfo, data, dataSize);
        return;
    }
    
    if (dataSize < sizeof(command_t))
    {
        ErrorLog("dataSize too smail - %d", dataSize);
        //sessionOut
        return ;
    }
    
    const char* pData = data;
    
    command_t cmd;
    memcpy(&cmd, data, sizeof(command_t));
    pData += sizeof(command_t);
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveHandler->receiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_CLIENT:
            clientReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            //sessionOut
            break;
    }
}




void IOManager::loginSessionIn(ConnectInfo* connectInfo)
{
    DebugLog("loginSessionIn");
}



void IOManager::loginSessionOut(ConnectInfo* connectInfo)
{
    DebugLog("loginSessionOut");

}




void IOManager::loginReceiveData(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DebugLog("loginReceiveData");
    
    DebugLog("data %d", dataSize);

    std::string dataString(data, dataSize);
    std::stringstream jsonString;
    jsonString << dataString;
    
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(jsonString, pt);
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt)
    {
        std::cout << v.second.data() << std::endl;
    }
}



///////////////////////////lobby recv

void IOManager::lobbySessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::lobbySessionOut(ConnectInfo* connectInfo)
{
    
}


void IOManager::lobbyReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    switch(cmd)
    {
        case LobbyFrontPacket::FIRST_CONNECT_REQ:
            lobbyHandleFirstConnectReq(connectInfo, data, dataSize);
            break;
            
        case LobbyFrontPacket::ENTER_CLIENT_RES:
            lobbyHandleEnterClientRes(connectInfo, data, dataSize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}



void IOManager::lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    LobbyFrontPacket::FirstConnectReqPacket packet;
    
    const char* pData = data;
    memcpy(packet.gameIp, pData, MAX_IP_ADDRESS_LEN);
    pData += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.gamePort, pData, sizeof(packet.gamePort));
    pData += sizeof(packet.gamePort);
    
    memcpy(packet.clientIp, pData, MAX_IP_ADDRESS_LEN);
    pData += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.clientPort, pData, sizeof(packet.clientPort));
    pData += sizeof(packet.clientPort);
    
    
    
    //memcpy(&packet + sizeof(command_t), data, dataSize);
    
    DebugLog("%d %d", packet.gamePort, packet.clientPort);
    
    if(FrontServer::getInstance()->serverInfoMgr->addLobbyServer(connectInfo, packet.gameIp, packet.gamePort, packet.clientIp, packet.clientPort) == false)
    {
        ErrorLog("??");
        return ;
    }
    
    
    //////////////////send response packet
    
    LobbyFrontPacket::FirstConnectResPacket sendPacket;
//    memcpy(sendPacket.chattingIp, FrontServer::getInstance()->serverInfoMgr->getChattingServerInfo()->lobbyIp, MAX_IP_ADDRESS_LEN);
//    sendPacket.chattingPort = FrontServer::getInstance()->serverInfoMgr->getChattingServerInfo()->lobbyPort;
    
    FrontServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::lobbyHandleEnterClientRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    LobbyFrontPacket::EnterClientResPacket packet;
    
    memcpy(&packet.userNo, data, sizeof(packet.userNo));
    
    User* user = FrontServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    ClientFrontPacket::EnterLobbyResPacket sendPacket;
    
    memcpy(sendPacket.ip, FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->clientIp, sizeof(sendPacket).ip);
    sendPacket.port = FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->clientPort;
    
    FrontServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));    
}


////////////////////////recv game

void IOManager::gameSessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::gameSessionOut(ConnectInfo* connectInfo)
{
    
}


void IOManager::gameReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    switch(cmd)
    {
        case GameFrontPacket::FIRST_CONNECT_REQ:
            gameHandleFirstConnectReq(connectInfo, data, dataSize);
            break;

        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::gameHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    GameFrontPacket::FirstConnectReqPacket packet;
    
    
    GameFrontPacket::FirstConnectResPacket sendPacket;

    
    
    memcpy(sendPacket.lobbyIp, FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->gameIp, MAX_IP_ADDRESS_LEN);
    sendPacket.lobbyPort = FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->gamePort;
    
    FrontServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}






////////////////////////client recv


void IOManager::clientSessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::clientSessionOut(ConnectInfo* connectInfo)
{
    User* user = (User*)connectInfo->userData;
    
    if(user != NULL)
    {
        if(user->getUserState() == USER_STATE_MOVING_LOBBY)
        {
            ///////////// send req to lobby
            
            LobbyFrontPacket::EnterClientOutPacket sendPacket;
            
            sendPacket.userNo = user->getUserNo();
            
            FrontServer::getInstance()->network->sendPacket(FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
        }
        
        ErrorLog("??");
        FrontServer::getInstance()->userMgr->removeUser(user);
    }
    
    connectInfo->userData = NULL;
    
}


void IOManager::clientReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    switch(cmd)
    {
        case ClientFrontPacket::FIRST_CONNECT_REQ:
            clientHandleFirstConnectReq(connectInfo, data, dataSize);
            break;
            
        case ClientFrontPacket::ENTER_LOBBY_REQ:
            clientHandleEnterLobbyReq(connectInfo, data, dataSize);
            break;
            
        case ClientFrontPacket::ENTER_LOBBY_OK:
            clientHandleEnterLobbyOk(connectInfo, data, dataSize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}

void IOManager::clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientFrontPacket::FirstConnectReqPacket packet;
    
    const char* pData = data;
    
    memcpy(&packet.sessionId, pData, sizeof(packet.sessionId));
    pData += sizeof(packet.sessionId);
    
    /////////////todo recv info from login server
    
    UserInfo userInfo;
    userInfo.userNo = createUserNo++;
    userInfo.nickNameLen = 5;
    sprintf(userInfo.nickName, "%s%d", "test", createUserNo);
    
    FrontServer::getInstance()->userMgr->addUnconnectedUser(&userInfo, &packet.sessionId);
    
    
    
    
    ///////////////////////////////
    
    if(FrontServer::getInstance()->userMgr->addConnectedUser(&packet.sessionId, connectInfo) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    ClientFrontPacket::FirstConnectResPacket sendPacket;
    
    sendPacket.userInfo.userNo = userInfo.userNo;
    sendPacket.userInfo.nickNameLen = userInfo.nickNameLen;
    memcpy(sendPacket.userInfo.nickName, userInfo.nickName, userInfo.nickNameLen);
    
    char* pSendBuffer = sendBuffer;
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.userInfo, sizeof(sendPacket.userInfo));
    pSendBuffer += sizeof(sendPacket.userInfo);
    
    FrontServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
}


void IOManager::clientHandleEnterLobbyReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    User* user = (User*)connectInfo->userData;
    
    user->setUserState(USER_STATE_MOVING_LOBBY);
    
    
    LobbyServerInfo* lobbyServerInfo = FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo();
    
    
    ///////////// send req to lobby
    
    LobbyFrontPacket::EnterClientReqPacket sendPacket;
    
    memcpy(&sendPacket.sid, user->getSid(), sizeof(SessionId_t));
    
    sendPacket.userInfo.userNo = user->getUserNo();
    sendPacket.userInfo.nickNameLen = user->getNickNameLen();
    memcpy(sendPacket.userInfo.nickName, user->getNickName(), user->getNickNameLen());
    //////////////TODO. all userInfo copy
    
    FrontServer::getInstance()->network->sendPacket(lobbyServerInfo->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::clientHandleEnterLobbyOk(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    User* user = (User*)connectInfo->userData;
    
    LobbyServerInfo* lobbyServerInfo = FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo();
    
    
    ///////////// send req to lobby
    
    LobbyFrontPacket::EnterClientOkPacket sendPacket;
    
    sendPacket.userNo = user->getUserNo();
    
    DebugLog("%d", user->getUserNo());
    FrontServer::getInstance()->network->sendPacket(lobbyServerInfo->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
    
    
    /////////TODO. sessionOut
    FrontServer::getInstance()->userMgr->removeUser(user);
    
    connectInfo->userData = NULL;
}






