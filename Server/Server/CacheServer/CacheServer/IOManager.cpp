#include "IOManager.h"
#include "Log.h"
#include "Network.h"

#include "CacheDefine.h"
#include "CacheServer.h"
#include "CacheData.h"

#include "FrontCachePacket.h"
#include "LobbyCachePacket.h"
#include "User.h"
#include "ServerInfo.h"
#include "GameDBChannel.h"
#include "Clan.h"


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
        case SERVER_MODULE_FRONT_SERVER:
            frontSessionIn(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionIn(connectInfo);
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
        case SERVER_MODULE_FRONT_SERVER:
            frontSessionOut(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionOut(connectInfo);
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
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveData(connectInfo, cmd, pData, pDataSize);
            break;
            
        default:
            ErrorLog("invalid server module - %d", connectInfo->serverModule);
            break;
    }
}


////////////////////////// front recv

void IOManager::frontSessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::frontSessionOut(ConnectInfo* connectInfo)
{
    
}


void IOManager::frontReceiveData(ConnectInfo* connectInfo, command_t cmd, const char* data, int dataSize)
{
    switch(cmd)
    {
        case FrontCachePacket::FIRST_CONNECT_REQ:
            frontHandleFirstConnectReq(connectInfo, data, dataSize);
            break;
            
        case FrontCachePacket::LOGIN_USER_REQ:
            frontHandleLoginUserReq(connectInfo, data, dataSize);
            break;
        case FrontCachePacket::CREATE_USER_REQ:
            frontHandleCreateUserReq(connectInfo, data, dataSize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::frontHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::FirstConnectReqPacket packet;
    
    FrontCachePacket::FirstConnectResPacket sendPacket;
    
    memcpy(sendPacket.lobbyIp, CacheServer::getInstance()->cacheData->getNetworkInfo(SERVER_MODULE_LOBBY_SERVER)->ip, MAX_IP_ADDRESS_LEN);
    
    
    sendPacket.lobbyPort = CacheServer::getInstance()->cacheData->getNetworkInfo(SERVER_MODULE_LOBBY_SERVER)->port;
    
    CacheServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::frontHandleLoginUserReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::LoginUserReqPacket packet;
    
    memcpy(&packet.sid, data, sizeof(packet.sid));
    data += sizeof(packet.sid);
    
    memcpy(&packet.userIdLen, data, sizeof(packet.userIdLen));
    data += sizeof(packet.userIdLen);
    
    memcpy(packet.userId, data, packet.userIdLen);
    data += packet.userIdLen;
    
    User* user = CacheServer::getInstance()->userMgr->getUserByUserId(std::string(packet.userId, packet.userIdLen));
    
    if(user == NULL)
    {
        FrontCachePacket::FailPacket failPacket;
        failPacket.cmd = FrontCachePacket::LOGIN_USER_FAIL;
        memcpy(&failPacket.sid, &packet.sid, sizeof(packet.sid));
        failPacket.failReason = FrontCachePacket::EXIST_USER;
        
        CacheServer::getInstance()->network->sendPacket(connectInfo, (char*)&failPacket, sizeof(failPacket));
        
        return ;
    }
    
    FrontCachePacket::LoginUserResPacket sendPacket;
    
    memcpy(&sendPacket.sid, &packet.sid, sizeof(packet.sid));
    sendPacket.userNo = user->getUserNo();
    
    CacheServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::frontHandleCreateUserReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    FrontCachePacket::CreateUserReqPacket packet;
    
    memcpy(&packet.sid, data, sizeof(packet.sid));
    data += sizeof(packet.sid);
    
    memcpy(&packet.userIdLen, data, sizeof(packet.userIdLen));
    data += sizeof(packet.userIdLen);
    
    memcpy(packet.userId, data, packet.userIdLen);
    data += packet.userIdLen;
    
    memcpy(&packet.nickNameLen, data, sizeof(packet.nickNameLen));
    data += sizeof(packet.nickNameLen);
    
    memcpy(packet.nickName, data, packet.nickNameLen);
    data += packet.nickNameLen;
    
    if(packet.nickNameLen == 0 || packet.userIdLen == 0)
    {
        ErrorLog("not good nickName");
    }
    
    User* user = CacheServer::getInstance()->userMgr->getUserByUserId(std::string(packet.userId, packet.userIdLen));
    
    if(user != NULL)
    {
        ErrorLog("exist user");
        return ;
    }
    
    NickNameInfo nickNameInfo;
    nickNameInfo.nickNameLen = packet.nickNameLen;
    memcpy(nickNameInfo.nickName, packet.nickName, packet.nickNameLen);
    
    user = CacheServer::getInstance()->userMgr->getUserByNickNameInfo(&nickNameInfo);
    
    if(user != NULL)
    {
        ErrorLog("exist user");
        return ;
    }
    
    int64_t userNo = CacheServer::getInstance()->gameDBChannel->InsertUser(std::string(packet.userId, packet.userIdLen), std::string(packet.nickName, packet.nickNameLen));
    
    if(userNo < 0)
    {
        ErrorLog("??");
        return ;
    }
    
    UserInfo userInfo;
    
    CacheServer::getInstance()->gameDBChannel->SelectUser(userNo, &userInfo);
    
    
    CacheServer::getInstance()->userMgr->addUser(std::string(packet.userId, packet.userIdLen), &userInfo);
    
    FrontCachePacket::CreateUserResPacket sendPacket;
    
    memcpy(&sendPacket.sid, &packet.sid, sizeof(sendPacket.sid));
    sendPacket.userNo = userNo;
    
    CacheServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
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
        case LobbyCachePacket::FIRST_CONNECT_REQ:
            lobbyHandleFirstConnectReq(connectInfo, data, dataSize);
            break;
            
        case LobbyCachePacket::LOGIN_USER_REQ:
            lobbyHandleLoginUserReq(connectInfo, data, dataSize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}



void IOManager::lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    LobbyCachePacket::FirstConnectReqPacket packet;
    
    //////////////////send response packet
    
    LobbyCachePacket::FirstConnectResPacket sendPacket;

    CacheServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::lobbyHandleLoginUserReq(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    LobbyCachePacket::LoginUserReqPacket packet;
    
    memcpy(&packet.userNo, data, sizeof(packet.userNo));
    
    User* user = CacheServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    LobbyCachePacket::LoginUserResPacket sendPacket;
    
    user->getUserInfo(&sendPacket.userInfo);
    
    sendPacket.friendCount = user->getFriendList(sendPacket.nickNameInfoList);
    
    if(user->getClanNo() != INVALID_CLAN_NO)
    {
        Clan* clan = CacheServer::getInstance()->clanMgr->getClanByClanNo(user->getClanNo());
        
        if(clan == NULL)
        {
            ErrorLog("clan not exist clan no = %d", user->getClanNo());
            return ;
        }
        
        ClanInfo* clanInfo = clan->getClanInfo();
        
        sendPacket.clanInfo.clanNo = clanInfo->clanNo;
        sendPacket.clanInfo.clanNameLen = clanInfo->clanNameLen;
        memcpy(sendPacket.clanInfo.clanName, clanInfo->clanName, clanInfo->clanNameLen);
        sendPacket.clanInfo.win = clanInfo->win;
        sendPacket.clanInfo.lose = clanInfo->lose;
        sendPacket.clanInfo.point = clanInfo->point;
    }
    else
    {
        sendPacket.clanInfo.clanNo = INVALID_CLAN_NO;
    }
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.userNo, sizeof(sendPacket.userInfo.userNo));
    pSendBuffer += sizeof(sendPacket.userInfo.userNo);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.nickNameLen, sizeof(sendPacket.userInfo.nickNameLen));
    pSendBuffer += sizeof(sendPacket.userInfo.nickNameLen);
    
    memcpy(pSendBuffer, sendPacket.userInfo.nickName, sendPacket.userInfo.nickNameLen);
    pSendBuffer += sendPacket.userInfo.nickNameLen;
    
    memcpy(pSendBuffer, &sendPacket.userInfo.commonWin, sizeof(sendPacket.userInfo.commonWin));
    pSendBuffer += sizeof(sendPacket.userInfo.commonWin);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.commonLose, sizeof(sendPacket.userInfo.commonLose));
    pSendBuffer += sizeof(sendPacket.userInfo.commonLose);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.commonDiss, sizeof(sendPacket.userInfo.commonDiss));
    pSendBuffer += sizeof(sendPacket.userInfo.commonDiss);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.rankWin, sizeof(sendPacket.userInfo.rankWin));
    pSendBuffer += sizeof(sendPacket.userInfo.rankWin);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.rankLose, sizeof(sendPacket.userInfo.rankLose));
    pSendBuffer += sizeof(sendPacket.userInfo.rankLose);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.rankDiss, sizeof(sendPacket.userInfo.rankDiss));
    pSendBuffer += sizeof(sendPacket.userInfo.rankDiss);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.grade, sizeof(sendPacket.userInfo.grade));
    pSendBuffer += sizeof(sendPacket.userInfo.grade);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.gradeReachedCount, sizeof(sendPacket.userInfo.gradeReachedCount));
    pSendBuffer += sizeof(sendPacket.userInfo.gradeReachedCount);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.point, sizeof(sendPacket.userInfo.point));
    pSendBuffer += sizeof(sendPacket.userInfo.point);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.coin, sizeof(sendPacket.userInfo.coin));
    pSendBuffer += sizeof(sendPacket.userInfo.coin);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.clanNo, sizeof(sendPacket.userInfo.clanNo));
    pSendBuffer += sizeof(sendPacket.userInfo.clanNo);
    
    memcpy(pSendBuffer, &sendPacket.userInfo.clanClass, sizeof(sendPacket.userInfo.clanClass));
    pSendBuffer += sizeof(sendPacket.userInfo.clanClass);
    
    memcpy(pSendBuffer, &sendPacket.friendCount, sizeof(sendPacket.friendCount));
    pSendBuffer += sizeof(sendPacket.friendCount);
    
    for(int i = 0; i < sendPacket.friendCount; i++)
    {
        memcpy(pSendBuffer, &sendPacket.nickNameInfoList[i].nickNameLen, sizeof(sendPacket.nickNameInfoList[i].nickNameLen));
        pSendBuffer += sizeof(sendPacket.nickNameInfoList[i].nickNameLen);
    
        memcpy(pSendBuffer, sendPacket.nickNameInfoList[i].nickName, sendPacket.nickNameInfoList[i].nickNameLen);
        pSendBuffer += sendPacket.nickNameInfoList[i].nickNameLen;
    }
    
    memcpy(pSendBuffer, &sendPacket.clanInfo.clanNo, sizeof(sendPacket.clanInfo.clanNo));
    pSendBuffer += sizeof(sendPacket.clanInfo.clanNo);
    
    if(user->getClanNo() != INVALID_CLAN_NO)
    {
        memcpy(pSendBuffer, &sendPacket.clanInfo.clanNameLen, sizeof(sendPacket.clanInfo.clanNameLen));
        pSendBuffer += sizeof(sendPacket.clanInfo.clanNameLen);
        
        memcpy(pSendBuffer, sendPacket.clanInfo.clanName, sendPacket.clanInfo.clanNameLen);
        pSendBuffer += sendPacket.clanInfo.clanNameLen;
        
        memcpy(pSendBuffer, &sendPacket.clanInfo.win, sizeof(sendPacket.clanInfo.win));
        pSendBuffer += sizeof(sendPacket.clanInfo.win);
        
        memcpy(pSendBuffer, &sendPacket.clanInfo.lose, sizeof(sendPacket.clanInfo.lose));
        pSendBuffer += sizeof(sendPacket.clanInfo.lose);
        
        memcpy(pSendBuffer, &sendPacket.clanInfo.point, sizeof(sendPacket.clanInfo.point));
        pSendBuffer += sizeof(sendPacket.clanInfo.point);
    }
    
    CacheServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
}


