#include "IOManager.h"
#include "Log.h"
#include "Network.h"

#include "FrontDefine.h"
#include "FrontServer.h"

#include "ClientFrontPacket.h"
#include "LobbyFrontPacket.h"
#include "GameFrontPacket.h"
#include "FrontCachePacket.h"
#include "User.h"
#include "ServerInfo.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>



IOManager::IOManager()
{
    createUserNo = 1;
}


IOManager::~IOManager()
{

}

void IOManager::connected(ConnectInfo* connectInfo)
{
    DebugLog("connected");
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_CACHE_SERVER:
            cacheSessionIn(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionIn(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameSessionIn(connectInfo);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingSessionIn(connectInfo);
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
        case SERVER_MODULE_CACHE_SERVER:
            cacheSessionOut(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionOut(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameSessionOut(connectInfo);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingSessionOut(connectInfo);
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
    if (dataSize < sizeof(command_t))
    {
        ErrorLog("dataSize too smail - %d", dataSize);
        return ;
    }
    
    const char* pData = data;
    int pDataSize = dataSize;
    
    command_t cmd;
    memcpy(&cmd, data, sizeof(command_t));
    pData += sizeof(command_t);
    pDataSize -= sizeof(command_t);
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_CACHE_SERVER:
            cacheReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
        case SERVER_MODULE_CLIENT:
            clientReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            break;
    }
}

void IOManager::cacheSessionIn(ConnectInfo* connectInfo)
{
    DebugLog("cache server session in");
    FrontCachePacket::FirstConnectReqPacket packet;
    FrontServer::getInstance()->network->sendPacket(connectInfo, (char*)&packet, sizeof(packet));
}

void IOManager::cacheSessionOut(ConnectInfo* connectInfo)
{
    DebugLog("cache server session out");
}

void IOManager::cacheReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    switch(cmd)
    {
        case FrontCachePacket::FIRST_CONNECT_RES:
            cacheHandleFirstConnectRes(connectInfo, data, dataSize);
            break;
            
        case FrontCachePacket::LOGIN_USER_RES:
            cacheHandleLoginUserRes(connectInfo, data, dataSize);
            break;
            
        case FrontCachePacket::LOGIN_USER_FAIL:
            cacheHandleLoginUserFail(connectInfo, data, dataSize);
            break;
            
        case FrontCachePacket::CREATE_USER_RES:
            cacheHandleCreateUserRes(connectInfo, data, dataSize);
            break;
            
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}

void IOManager::cacheHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::FirstConnectResPacket packet;
    memcpy(packet.lobbyIp, data, MAX_IP_ADDRESS_LEN);
    memcpy(&packet.lobbyPort, data + MAX_IP_ADDRESS_LEN, sizeof(packet.lobbyPort));
    
    DebugLog("cache lobby connect %s %d", packet.lobbyIp, packet.lobbyPort);
    
    if(FrontServer::getInstance()->serverInfoMgr->addCacheServer(connectInfo, packet.lobbyIp, packet.lobbyPort) == false)
    {
        ErrorLog("??");
        return ;
    }
}

void IOManager::cacheHandleLoginUserRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::LoginUserResPacket packet;
    memcpy(&packet.sid, data, sizeof(packet.sid));
    memcpy(&packet.userNo, data + sizeof(packet.sid), sizeof(packet.userNo));
    
    FrontServer::getInstance()->userMgr->addConnectedUser(&packet.sid, packet.userNo);
    
    User* user = FrontServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
    
    if(user == NULL)
    {
        DebugLog("leave already");
        return ;
    }
    
    //////////////////////////////////////
    
    ClientFrontPacket::LoginResPacket sendPacket;
    
    sendPacket.loginInfo = ClientFrontPacket::LOGIN_RESULT_SUCCESS;
    
    FrontServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::cacheHandleLoginUserFail(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::FailPacket packet;
    memcpy(&packet.sid, data, sizeof(packet.sid));
    memcpy(&packet.failReason, data + sizeof(packet.sid), sizeof(packet.failReason));
    
    User* user = FrontServer::getInstance()->userMgr->getUnConnectedUser(&packet.sid);
    
    if(user == NULL)
    {
        DebugLog("leave already");
        return ;
    }
    
    //////////////////////////////////////
    
    ClientFrontPacket::LoginResPacket sendPacket;
    
    if(packet.failReason == FrontCachePacket::EXIST_USER)
    {
        sendPacket.loginInfo = ClientFrontPacket::LOGIN_RESULT_NOT_EXIST_USER_ID;
    }
    else
    {
        ErrorLog("??");
        return ;
    }
    
    
    FrontServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::cacheHandleCreateUserRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::CreateUserResPacket packet;
    memcpy(&packet.sid, data, sizeof(packet.sid));
    memcpy(&packet.userNo, data + sizeof(packet.sid), sizeof(packet.userNo));
    
    FrontServer::getInstance()->userMgr->addConnectedUser(&packet.sid, packet.userNo);
    
    User* user = FrontServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
    
    if(user == NULL)
    {
        DebugLog("leave already");
        return ;
    }
    
    //////////////////////////////////////
    
    ClientFrontPacket::CreateAccountResPacket sendPacket;
    
    sendPacket.isSuccess = ClientFrontPacket::CREATE_ACCOUNT_RESULT_SUCCESS;
    
    FrontServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
}

///////////////////////////lobby recv

void IOManager::lobbySessionIn(ConnectInfo* connectInfo)
{
    DebugLog("lobby server session in");
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
    
    DebugLog("packet.gameIp = %s, packet.gamePort = %d, packet.clientIp = %s packet.clientPort = %d", packet.gameIp, packet.gamePort, packet.clientIp, packet.clientPort);
    
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
    
    if(user == NULL) // 유저가 req 보낸 이후에 바로 연결을 끊어버릴 경우
    {
        DebugLog("user out - userNo", packet.userNo);
        
        LobbyFrontPacket::EnterClientOutPacket sendPacket;
        
        sendPacket.userNo = packet.userNo;
        
        DebugLog("%d", user->getUserNo());
        FrontServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
        
        return ;
    }
    
    LobbyServerInfo* lobbyServerInfo = (LobbyServerInfo*)connectInfo->userData;
    
    ClientFrontPacket::EnterLobbyResPacket sendPacket;
    
    memcpy(sendPacket.ip, lobbyServerInfo->clientIp, MAX_IP_ADDRESS_LEN);
    sendPacket.port = lobbyServerInfo->clientPort;
    
    FrontServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));    
}


////////////////////////recv game

void IOManager::gameSessionIn(ConnectInfo* connectInfo)
{
    DebugLog("gameSessionIn");
}


void IOManager::gameSessionOut(ConnectInfo* connectInfo)
{
    DebugLog("gameSessionOut");
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
    GameFrontPacket::FirstConnectResPacket sendPacket;
    
    int lobbyCount = FrontServer::getInstance()->serverInfoMgr->getLobbyCount();
    
    if(lobbyCount != 1)
    {
        ErrorLog("lobbyCount - %d", lobbyCount);
        return ;
    }
    
    for(int i = 0; i < lobbyCount; i++)
    {
        memcpy(sendPacket.lobbyIp, FrontServer::getInstance()->serverInfoMgr->getRandomLobbyServerInfo()->gameIp, MAX_IP_ADDRESS_LEN);
        sendPacket.lobbyPort = FrontServer::getInstance()->serverInfoMgr->getRandomLobbyServerInfo()->gamePort;
    }
    
    FrontServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


////////////////////////chatting recv

void IOManager::chattingSessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::chattingSessionOut(ConnectInfo* connectInfo)
{
    
}


void IOManager::chattingReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    
}


void IOManager::chattingHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
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
        if(user->getUserState() == USER_STATE_NOT_LOGIN)
        {
            //TODO.remove user in unconnectedUser
        }
        else
        {
            if(user->getUserState() == USER_STATE_MOVING_LOBBY)
            {
                ///////////// send req to lobby
                
                LobbyFrontPacket::EnterClientOutPacket sendPacket;
                
                sendPacket.userNo = user->getUserNo();
                
                int lobbyNo = user->getLobbyNo();
                
                LobbyServerInfo* lobbyServerInfo = FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo(lobbyNo);
                
                FrontServer::getInstance()->network->sendPacket(lobbyServerInfo->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
            }
            
            FrontServer::getInstance()->userMgr->removeUser(user);
            
            connectInfo->userData = NULL;
        }
    }
}


void IOManager::clientReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    switch(cmd)
    {
        case ClientFrontPacket::FIRST_CONNECT_REQ:
            clientHandleFirstConnectReq(connectInfo, data, dataSize);
            break;
            
        case ClientFrontPacket::LOGIN_REQ:
            clientHandleLoginReq(connectInfo, data, dataSize);
            break;
            
        case ClientFrontPacket::CREATE_ACCOUNT_REQ:
            clientHandleCreateAccountReq(connectInfo, data, dataSize);
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
    DebugLog("clientHandleFirstConnectReq");
    ClientFrontPacket::FirstConnectReqPacket packet;
    
    memcpy(&packet.packetVersion, data, sizeof(packet.packetVersion));
    
    //TODO. packet version check
    
    SessionId_t sessionId;
    
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    
    const std::string tmp = boost::lexical_cast<std::string>(uuid);
    const char * value = tmp.c_str();
    
    
    memcpy(&sessionId, value, sizeof(sessionId));
    
    FrontServer::getInstance()->userMgr->addUnconnectedUser(&sessionId, connectInfo);
    
    
    
    
    ///////////////////////////////
    
    ClientFrontPacket::FirstConnectResPacket sendPacket;
    
    memcpy(&sendPacket.sessionId, &sessionId, sizeof(sessionId));
    
    FrontServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::clientHandleLoginReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientFrontPacket::LoginReqPacket packet;
    memcpy(&packet.userIdLen, data, sizeof(packet.userIdLen));
    data += sizeof(packet.userIdLen);
    
    memcpy(packet.userId, data, packet.userIdLen);
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    FrontCachePacket::LoginUserReqPacket sendPacket;
    
    memcpy(&sendPacket.sid, user->getSid(), sizeof(sendPacket.sid));
    sendPacket.userIdLen = packet.userIdLen;
    memcpy(sendPacket.userId, packet.userId, packet.userIdLen);
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.sid, sizeof(sendPacket.sid));
    pSendBuffer += sizeof(sendPacket.sid);
    
    memcpy(pSendBuffer, &sendPacket.userIdLen, sizeof(sendPacket.userIdLen));
    pSendBuffer += sizeof(sendPacket.userIdLen);
    
    memcpy(pSendBuffer, sendPacket.userId, sendPacket.userIdLen);
    pSendBuffer += sendPacket.userIdLen;
    
    FrontServer::getInstance()->network->sendPacket(FrontServer::getInstance()->serverInfoMgr->getCacheServerInfo()->connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
}

void IOManager::clientHandleCreateAccountReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientFrontPacket::CreateAccountReqPacket packet;
    memcpy(&packet.userIdLen, data, sizeof(packet.userIdLen));
    data += sizeof(packet.userIdLen);
    memcpy(packet.userId, data, packet.userIdLen);
    data += packet.userIdLen;
    memcpy(&packet.nickNameLen, data, sizeof(packet.nickNameLen));
    data += sizeof(packet.nickNameLen);
    memcpy(packet.nickName, data, packet.nickNameLen);
    data += packet.nickNameLen;
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    FrontCachePacket::CreateUserReqPacket sendPacket;
    
    memcpy(&sendPacket.sid, user->getSid(), sizeof(sendPacket.sid));
    sendPacket.userIdLen = packet.userIdLen;
    memcpy(sendPacket.userId, packet.userId, packet.userIdLen);
    sendPacket.nickNameLen = packet.nickNameLen;
    memcpy(sendPacket.nickName, packet.nickName, packet.nickNameLen);
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.sid, sizeof(sendPacket.sid));
    pSendBuffer += sizeof(sendPacket.sid);
    
    memcpy(pSendBuffer, &sendPacket.userIdLen, sizeof(sendPacket.userIdLen));
    pSendBuffer += sizeof(sendPacket.userIdLen);
    
    memcpy(pSendBuffer, sendPacket.userId, sendPacket.userIdLen);
    pSendBuffer += sendPacket.userIdLen;
    
    memcpy(pSendBuffer, &sendPacket.nickNameLen, sizeof(sendPacket.nickNameLen));
    pSendBuffer += sizeof(sendPacket.nickNameLen);
    
    memcpy(pSendBuffer, sendPacket.nickName, sendPacket.nickNameLen);
    pSendBuffer += sendPacket.nickNameLen;
    
    FrontServer::getInstance()->network->sendPacket(FrontServer::getInstance()->serverInfoMgr->getCacheServerInfo()->connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
}


void IOManager::clientHandleEnterLobbyReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DebugLog("clientHandleEnterLobbyReq");
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        DebugLog("not exist user");
        return ;
    }
    
    if(user->getUserState() == USER_STATE_MOVING_LOBBY)
    {
        ErrorLog("already moving");
        return ;
    }
    
    user->setUserState(USER_STATE_MOVING_LOBBY);
    
    
    LobbyServerInfo* lobbyServerInfo = FrontServer::getInstance()->serverInfoMgr->getRandomLobbyServerInfo();
    
    
    ///////////// send req to lobby
    
    LobbyFrontPacket::EnterClientReqPacket sendPacket;
    
    memcpy(&sendPacket.sid, user->getSid(), sizeof(SessionId_t));
    
    sendPacket.userNo = user->getUserNo();
    
    FrontServer::getInstance()->network->sendPacket(lobbyServerInfo->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
    
    user->setLobbyNo(lobbyServerInfo->lobbyNo);
}

void IOManager::clientHandleEnterLobbyOk(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    User* user = (User*)connectInfo->userData;
    
    LobbyServerInfo* lobbyServerInfo = FrontServer::getInstance()->serverInfoMgr->getLobbyServerInfo(user->getLobbyNo());
    
    
    ///////////// send req to lobby
    
    LobbyFrontPacket::EnterClientOkPacket sendPacket;
    
    sendPacket.userNo = user->getUserNo();
    
    FrontServer::getInstance()->network->sendPacket(lobbyServerInfo->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
    
    
    /////////TODO. sessionOut
    FrontServer::getInstance()->userMgr->removeUser(user);
    
    connectInfo->userData = NULL;
}






