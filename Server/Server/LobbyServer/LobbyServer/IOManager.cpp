#include "IOManager.h"
#include "Log.h"
#include "Network.h"
#include "User.h"
#include "Channel.h"
#include "ClientLobbyPacket.h"
#include "LobbyFrontPacket.h"
#include "GameLobbyPacket.h"
#include "LobbyChattingPacket.h"
#include "LobbyCachePacket.h"
#include "Room.h"

#include "LobbyDefine.h"
#include "LobbyServer.h"
#include "LobbyData.h"
#include "ServerInfo.h"
#include "QuickPlay.h"
#include "Clan.h"


IOManager::IOManager()
{

}


IOManager::~IOManager()
{

}

void IOManager::connected(ConnectInfo* connectInfo)
{
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_CACHE_SERVER:
            cacheSessionIn(connectInfo);
            break;
        case SERVER_MODULE_FRONT_SERVER:
            frontSessionIn(connectInfo);
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
    LobbyServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
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
        case SERVER_MODULE_FRONT_SERVER:
            frontSessionOut(connectInfo);
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
    LobbyServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
#endif
}


void IOManager::receiveData(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    DebugLog("reciveData");
    
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
        case SERVER_MODULE_CACHE_SERVER:
            cacheReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_CHATTING_SERVER:
            chattingReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
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




//////////////////////////////////////// cache recv


void IOManager::cacheReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case LobbyCachePacket::FIRST_CONNECT_RES:
            cacheHandleFirstConnectRes(connectInfo, body, bodySize);
            break;
            
        case LobbyCachePacket::LOGIN_USER_RES:
            cacheHandleLoginUserRes(connectInfo, body, bodySize);
            break;

        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::cacheSessionIn(ConnectInfo* connectInfo)
{
    DebugLog("cache session in");

    LobbyCachePacket::FirstConnectReqPacket sendPacket;
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::cacheSessionOut(ConnectInfo* connectInfo)
{
    ErrorLog("never disconnect");

}


void IOManager::cacheHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("cacheHandleFirstConnectRes");

    if(LobbyServer::getInstance()->serverInfoMgr->addCacheServer(connectInfo) == false)
    {
        ErrorLog("??");
        return ;
    }
}


void IOManager::cacheHandleLoginUserRes(ConnectInfo* connectInfo, const char* data, int bodySize)
{
    LobbyCachePacket::LoginUserResPacket packet;
    
    memcpy(&packet.userInfo.userNo, data, sizeof(packet.userInfo.userNo));
    data += sizeof(packet.userInfo.userNo);
    
    memcpy(&packet.userInfo.nickNameLen, data, sizeof(packet.userInfo.nickNameLen));
    data += sizeof(packet.userInfo.nickNameLen);
    
    memcpy(packet.userInfo.nickName, data, packet.userInfo.nickNameLen);
    data += packet.userInfo.nickNameLen;
    
    memcpy(&packet.userInfo.commonWin, data, sizeof(packet.userInfo.commonWin));
    data += sizeof(packet.userInfo.commonWin);
    
    memcpy(&packet.userInfo.commonLose, data, sizeof(packet.userInfo.commonLose));
    data += sizeof(packet.userInfo.commonLose);
    
    memcpy(&packet.userInfo.commonDiss, data, sizeof(packet.userInfo.commonDiss));
    data += sizeof(packet.userInfo.commonDiss);
    
    memcpy(&packet.userInfo.rankWin, data, sizeof(packet.userInfo.rankWin));
    data += sizeof(packet.userInfo.rankWin);
    
    memcpy(&packet.userInfo.rankLose, data, sizeof(packet.userInfo.rankLose));
    data += sizeof(packet.userInfo.rankLose);
    
    memcpy(&packet.userInfo.rankDiss, data, sizeof(packet.userInfo.rankDiss));
    data += sizeof(packet.userInfo.rankDiss);
    
    memcpy(&packet.userInfo.grade, data, sizeof(packet.userInfo.grade));
    data += sizeof(packet.userInfo.grade);
    
    memcpy(&packet.userInfo.gradeReachedCount, data, sizeof(packet.userInfo.gradeReachedCount));
    data += sizeof(packet.userInfo.gradeReachedCount);
    
    memcpy(&packet.userInfo.point, data, sizeof(packet.userInfo.point));
    data += sizeof(packet.userInfo.point);
    
    memcpy(&packet.userInfo.coin, data, sizeof(packet.userInfo.coin));
    data += sizeof(packet.userInfo.coin);
    
    memcpy(&packet.userInfo.clanNo, data, sizeof(packet.userInfo.clanNo));
    data += sizeof(packet.userInfo.clanNo);
    
    memcpy(&packet.userInfo.clanClass, data, sizeof(packet.userInfo.clanClass));
    data += sizeof(packet.userInfo.clanClass);
    
    memcpy(&packet.friendCount, data, sizeof(packet.friendCount));
    data += sizeof(packet.friendCount);
    
    for(int i = 0; i < packet.friendCount; i++)
    {
        memcpy(&packet.nickNameInfoList[i].nickNameLen, data, sizeof(packet.nickNameInfoList[i].nickNameLen));
        data += sizeof(packet.nickNameInfoList[i].nickNameLen);
        
        memcpy(packet.nickNameInfoList[i].nickName, data, packet.nickNameInfoList[i].nickNameLen);
        data += packet.nickNameInfoList[i].nickNameLen;
    }
    
    memcpy(&packet.clanInfo.clanNo, data, sizeof(packet.clanInfo.clanNo));
    data += sizeof(packet.clanInfo.clanNo);
    
    if(packet.clanInfo.clanNo != INVALID_CLAN_NO)
    {
        memcpy(&packet.clanInfo.clanNameLen, data, sizeof(packet.clanInfo.clanNameLen));
        data += sizeof(packet.clanInfo.clanNameLen);
        
        memcpy(packet.clanInfo.clanName, data, sizeof(packet.clanInfo.clanName));
        data += sizeof(packet.clanInfo.clanName);
        
        memcpy(&packet.clanInfo.win, data, sizeof(packet.clanInfo.win));
        data += sizeof(packet.clanInfo.win);
        
        memcpy(&packet.clanInfo.lose, data, sizeof(packet.clanInfo.lose));
        data += sizeof(packet.clanInfo.lose);
        
        memcpy(&packet.clanInfo.point, data, sizeof(packet.clanInfo.point));
        data += sizeof(packet.clanInfo.point);
    }
    
    User* user = LobbyServer::getInstance()->userMgr->getUnconnectedUserByUserNo(packet.userInfo.userNo);
    
    if(user == NULL)
    {
        DebugLog("already out");
        return ;
    }
    
    if(LobbyServer::getInstance()->userMgr->addConnectedUser(&packet.userInfo, packet.friendCount, packet.nickNameInfoList, &packet.clanInfo) != SUCCESS)
    {
        NickNameInfo nickNameInfo;
        nickNameInfo.nickNameLen = packet.userInfo.nickNameLen;
        memcpy(nickNameInfo.nickName, packet.userInfo.nickName, sizeof(nickNameInfo.nickNameLen));
        LobbyServer::getInstance()->userMgr->removeUserByNickNameInfo(&nickNameInfo);
        if(LobbyServer::getInstance()->userMgr->addConnectedUser(&packet.userInfo, packet.friendCount, packet.nickNameInfoList, &packet.clanInfo) != SUCCESS)
        {
            ErrorLog("fuck");
            
        }
        
    }
    
    
    LobbyChattingPacket::EnterClientReqPacket sendPacket;
    
    memcpy(&sendPacket.sid, user->getSid(), sizeof(sendPacket.sid));
    sendPacket.userNo = user->getUserNo();
    sendPacket.nickNameInfo.nickNameLen = user->getNickNameLen();
    memcpy(sendPacket.nickNameInfo.nickName, user->getNickName(), user->getNickNameLen());
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.sid, sizeof(sendPacket.sid));
    pSendBuffer += sizeof(sendPacket.sid);
    
    memcpy(pSendBuffer, &sendPacket.userNo, sizeof(sendPacket.userNo));
    pSendBuffer += sizeof(sendPacket.userNo);
    
    memcpy(pSendBuffer, &sendPacket.nickNameInfo.nickNameLen, sizeof(sendPacket.nickNameInfo.nickNameLen));
    pSendBuffer += sizeof(sendPacket.nickNameInfo.nickNameLen);
    
    memcpy(pSendBuffer, &sendPacket.nickNameInfo.nickName, sendPacket.nickNameInfo.nickNameLen);
    pSendBuffer += sendPacket.nickNameInfo.nickNameLen;
    
    ChattingServerInfo* chattingServerInfo = LobbyServer::getInstance()->serverInfoMgr->getChattingServerInfo();
    
    if(chattingServerInfo->connectInfo == NULL)
    {
        ErrorLog("chatting server fuck");
        return ;
    }
    
    LobbyServer::getInstance()->network->sendPacket(chattingServerInfo->connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));

//    ChattingServerInfo* chattingServerInfo = LobbyServer::getInstance()->serverInfoMgr->getChattingServerInfo();
//    
//    if(chattingServerInfo == NULL)
//    {
//        ErrorLog("chatting server not exist");
//        return ;
//    }
//    
//    ClientLobbyPacket::FirstConnectResPacket sendPacket;
//    memcpy(sendPacket.chattingIp, chattingServerInfo->clientIp, MAX_IP_ADDRESS_LEN);
//    sendPacket.chattingPort = chattingServerInfo->clientPort;
//        
//    user->getUserInfo(&sendPacket.userInfo);
//    
//    user->getFriendList(sendPacket.nickNameInfoWithOnlineList);
//    
//    if(user->getClanNo() != INVALID_CLAN_NO)
//    {
//        Clan* clan = LobbyServer::getInstance()->clanMgr->getClanByClanNo(user->getClanNo());
//        
//        if(clan != NULL)
//        {
//            ErrorLog("clan not exist clan no = %d", user->getClanNo());
//            return ;
//        }
//        
//        ClanInfo* clanInfo = clan->getClanInfo();
//        
//        sendPacket.clanInfo.clanNo = clanInfo->clanNo;
//        sendPacket.clanInfo.clanNameLen = clanInfo->clanNameLen;
//        memcpy(sendPacket.clanInfo.clanName, clanInfo->clanName, clanInfo->clanNameLen);
//        sendPacket.clanInfo.win = clanInfo->win;
//        sendPacket.clanInfo.lose = clanInfo->lose;
//        sendPacket.clanInfo.point = clanInfo->point;
//    }
//    else
//    {
//        sendPacket.clanInfo.clanNo = INVALID_CLAN_NO;
//    }
//    
//    char* pSendBuffer = sendBuffer;
//    
//    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
//    pSendBuffer += sizeof(sendPacket.cmd);
//    
//    memcpy(pSendBuffer, &sendPacket.chattingIp, MAX_IP_ADDRESS_LEN);
//    pSendBuffer += MAX_IP_ADDRESS_LEN;
//    
//    memcpy(pSendBuffer, &sendPacket.chattingPort, sizeof(sendPacket.chattingPort));
//    pSendBuffer += sizeof(sendPacket.chattingPort);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.userNo, sizeof(sendPacket.userInfo.userNo));
//    pSendBuffer += sizeof(sendPacket.userInfo.userNo);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.nickNameLen, sizeof(sendPacket.userInfo.nickNameLen));
//    pSendBuffer += sizeof(sendPacket.userInfo.nickNameLen);
//    
//    memcpy(pSendBuffer, sendPacket.userInfo.nickName, sendPacket.userInfo.nickNameLen);
//    pSendBuffer += sendPacket.userInfo.nickNameLen;
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.commonWin, sizeof(sendPacket.userInfo.commonWin));
//    pSendBuffer += sizeof(sendPacket.userInfo.commonWin);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.commonLose, sizeof(sendPacket.userInfo.commonLose));
//    pSendBuffer += sizeof(sendPacket.userInfo.commonLose);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.commonDiss, sizeof(sendPacket.userInfo.commonDiss));
//    pSendBuffer += sizeof(sendPacket.userInfo.commonDiss);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.rankWin, sizeof(sendPacket.userInfo.rankWin));
//    pSendBuffer += sizeof(sendPacket.userInfo.rankWin);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.rankLose, sizeof(sendPacket.userInfo.rankLose));
//    pSendBuffer += sizeof(sendPacket.userInfo.rankLose);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.rankDiss, sizeof(sendPacket.userInfo.rankDiss));
//    pSendBuffer += sizeof(sendPacket.userInfo.rankDiss);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.grade, sizeof(sendPacket.userInfo.grade));
//    pSendBuffer += sizeof(sendPacket.userInfo.grade);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.gradeReachedCount, sizeof(sendPacket.userInfo.gradeReachedCount));
//    pSendBuffer += sizeof(sendPacket.userInfo.gradeReachedCount);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.point, sizeof(sendPacket.userInfo.point));
//    pSendBuffer += sizeof(sendPacket.userInfo.point);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.coin, sizeof(sendPacket.userInfo.coin));
//    pSendBuffer += sizeof(sendPacket.userInfo.coin);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.clanNo, sizeof(sendPacket.userInfo.clanNo));
//    pSendBuffer += sizeof(sendPacket.userInfo.clanNo);
//    
//    memcpy(pSendBuffer, &sendPacket.userInfo.clanClass, sizeof(sendPacket.userInfo.clanClass));
//    pSendBuffer += sizeof(sendPacket.userInfo.clanClass);
//    
//    memcpy(pSendBuffer, &sendPacket.friendCount, sizeof(sendPacket.friendCount));
//    pSendBuffer += sizeof(sendPacket.friendCount);
//    
//    for(int i = 0; i < sendPacket.friendCount; i++)
//    {
//        memcpy(pSendBuffer, &sendPacket.nickNameInfoWithOnlineList[i].online, sizeof(sendPacket.nickNameInfoWithOnlineList[i].online));
//        pSendBuffer += sizeof(sendPacket.nickNameInfoWithOnlineList[i].online);
//        
//        memcpy(pSendBuffer, &sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen, sizeof(sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen));
//        pSendBuffer += sizeof(sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen);
//        
//        memcpy(pSendBuffer, sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickName, sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen);
//        pSendBuffer += sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen;
//    }
//    
//    memcpy(pSendBuffer, &sendPacket.clanInfo.clanNo, sizeof(sendPacket.clanInfo.clanNo));
//    pSendBuffer += sizeof(sendPacket.clanInfo.clanNo);
//    
//    if(user->getClanNo() != INVALID_CLAN_NO)
//    {
//        memcpy(pSendBuffer, &sendPacket.clanInfo.clanNameLen, sizeof(sendPacket.clanInfo.clanNameLen));
//        pSendBuffer += sizeof(sendPacket.clanInfo.clanNameLen);
//        
//        memcpy(pSendBuffer, sendPacket.clanInfo.clanName, sendPacket.clanInfo.clanNameLen);
//        pSendBuffer += sendPacket.clanInfo.clanNameLen;
//        
//        memcpy(pSendBuffer, &sendPacket.clanInfo.win, sizeof(sendPacket.clanInfo.win));
//        pSendBuffer += sizeof(sendPacket.clanInfo.win);
//        
//        memcpy(pSendBuffer, &sendPacket.clanInfo.lose, sizeof(sendPacket.clanInfo.lose));
//        pSendBuffer += sizeof(sendPacket.clanInfo.lose);
//        
//        memcpy(pSendBuffer, &sendPacket.clanInfo.point, sizeof(sendPacket.clanInfo.point));
//        pSendBuffer += sizeof(sendPacket.clanInfo.point);
//    }
//    
//    LobbyServer::getInstance()->network->sendPacket(user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
    
}



//////////////////////////////////////// front recv

void IOManager::frontReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case LobbyFrontPacket::FIRST_CONNECT_RES:
            frontHandleFirstConnectRes(connectInfo, body, bodySize);
            break;
            
        case LobbyFrontPacket::ENTER_CLIENT_REQ:
            frontHandleEnterClientReq(connectInfo, body, bodySize);
            break;

        case LobbyFrontPacket::ENTER_CLIENT_OK:
            frontHandleEnterClientOk(connectInfo, body, bodySize);
            break;

        case LobbyFrontPacket::ENTER_CLIENT_OUT:
            frontHandleEnterClientOut(connectInfo, body, bodySize);
            break;

        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::frontSessionIn(ConnectInfo* connectInfo)
{
    LobbyFrontPacket::FirstConnectReqPacket packet;
    
    memcpy(packet.gameIp, LobbyServer::getInstance()->lobbyData->getNetworkInfo(SERVER_MODULE_GAME_SERVER)->ip, MAX_IP_ADDRESS_LEN);
    packet.gamePort = LobbyServer::getInstance()->lobbyData->getNetworkInfo(SERVER_MODULE_GAME_SERVER)->port;
    
    memcpy(packet.clientIp, LobbyServer::getInstance()->lobbyData->getNetworkInfo(SERVER_MODULE_CLIENT)->ip, MAX_IP_ADDRESS_LEN);
    packet.clientPort = LobbyServer::getInstance()->lobbyData->getNetworkInfo(SERVER_MODULE_CLIENT)->port;
    
    DebugLog("%s %d %s %d", packet.gameIp, packet.gamePort, packet.clientIp, packet.clientPort);
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&packet, sizeof(packet));
}


void IOManager::frontSessionOut(ConnectInfo* connectInfo)
{
    ErrorLog("never disconnect");
}


void IOManager::frontHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    LobbyFrontPacket::FirstConnectResPacket packet;
    
    memcpy(packet.chattingIp, body, MAX_IP_ADDRESS_LEN);
    body += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.chattingPort, body, sizeof(packet.chattingPort));
    body += sizeof(packet.chattingPort);
    
    /////////////TODO. connect with chatting server
}


void IOManager::frontHandleEnterClientReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    LobbyFrontPacket::EnterClientReqPacket packet;
    
    const char* pBody = body;
    
    memcpy(&packet.sid, pBody, sizeof(packet.sid));
    pBody += sizeof(packet.sid);
    
    memcpy(&packet.userNo, pBody, sizeof(packet.userNo));
    pBody += sizeof(packet.userNo);
    
    if(LobbyServer::getInstance()->userMgr->addUnconnectedUser(&packet.sid, packet.userNo) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    
    
    /////////////// sendPacket
    
    LobbyFrontPacket::EnterClientResPacket sendPacket;
    
    sendPacket.userNo = packet.userNo;
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}



void IOManager::frontHandleEnterClientOk(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    LobbyFrontPacket::EnterClientOkPacket packet;
    
    const char* pBody = body;
    
    memcpy(&packet.userNo, pBody, sizeof(packet.userNo));
    pBody += sizeof(packet.userNo);
    
    DebugLog("%d", packet.userNo);
    
    User* user = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
    
    if(user == NULL)
    {
        ErrorLog("not exist user");
        //???
        return ;
    }
    
    ///////////// send packet
    
    ClientLobbyPacket::FirstConnectOkPacket sendPacket;
    
    LobbyServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::frontHandleEnterClientOut(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    LobbyFrontPacket::EnterClientOutPacket packet;
    
    const char* pBody = body;
    
    memcpy(&packet.userNo, pBody, sizeof(packet.userNo));
    pBody += sizeof(packet.userNo);
    
    
    if(LobbyServer::getInstance()->userMgr->removeUnconnectedUserByUserNo(packet.userNo) == false)
    {
        User* user = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
        
        if(user == NULL)
        {
            ErrorLog("??");
            return ;
        }
        
        ///////////// send packet
        
        ClientLobbyPacket::FirstConnectOutPacket sendPacket;
        
        LobbyServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
    }
    
}


//////////////////////////////////////// lobby recv

void IOManager::lobbyReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    
}


void IOManager::lobbySessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::lobbySessionOut(ConnectInfo* connectInfo)
{
    
}



void IOManager::lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    
}





//////////////////////////////////////// game recv

void IOManager::gameReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case GameLobbyPacket::FIRST_CONNECT_REQ:
            gameHandleFirstConnectReq(connectInfo, body, bodySize);
            break;
            
        case GameLobbyPacket::ENTER_ROOM_RES:
            gameHandleEnterRoomRes(connectInfo, body, bodySize);
            break;
            
        case GameLobbyPacket::FINISH_GAME_REQ:
            gameHandleFinishGameReq(connectInfo, body, bodySize);
            break;
        
        case GameLobbyPacket::MOVING_CLIENT_DISCONNCET:
            gameHandleMovingClientDisconnect(connectInfo, body, bodySize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::gameSessionIn(ConnectInfo* connectInfo)
{
    DebugLog("gameSessionIn");
}


void IOManager::gameSessionOut(ConnectInfo* connectInfo)
{
    DebugLog("game sessionOut");

    GameServerInfo* gameServerInfo = (GameServerInfo*)connectInfo->userData;
    
    if(gameServerInfo == NULL)
    {
        ErrorLog("gameServerInfo not login");
    }
    else
    {
        if(LobbyServer::getInstance()->serverInfoMgr->removeGameServer(gameServerInfo->gameServerNo) == false)
        {
            ErrorLog("game server remove fail");
        }
        else
        {
            DebugLog("game server remove");
        }
    }
}


void IOManager::gameHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("game First connect req");
    
    GameLobbyPacket::FirstConnectReqPacket packet;
    
    const char* pBody = body;
    memcpy(packet.clientIp, pBody, MAX_IP_ADDRESS_LEN);
    pBody += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.clientPort, pBody, sizeof(packet.clientPort));
    pBody += sizeof(packet.clientPort);
    
    
    LobbyServer::getInstance()->serverInfoMgr->addGameServer(connectInfo, packet.clientIp, packet.clientPort);
    
    GameLobbyPacket::FirstConnectResPacket sendPacket;
    
    memcpy(sendPacket.clientIp, LobbyServer::getInstance()->lobbyData->getNetworkInfo(SERVER_MODULE_CLIENT)->ip, MAX_IP_ADDRESS_LEN);
    sendPacket.clientPort = LobbyServer::getInstance()->lobbyData->getNetworkInfo(SERVER_MODULE_CLIENT)->port;
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::gameHandleEnterRoomRes(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::EnterRoomResPacket packet;
    
    memcpy(&packet.roomNo, body, sizeof(packet.roomNo));
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(packet.roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    GameServerInfo* gameServerInfo = (GameServerInfo*)connectInfo->userData;
    
    if(gameServerInfo == NULL)
    {
        ErrorLog("gameSreverInfo is null");
        return ;
    }
    
    room->startGameNotify(gameServerInfo);
}


void IOManager::gameHandleFinishGameReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::FinishGameReqPacket packet;
    
    memcpy(&packet.roomNo, body, sizeof(packet.roomNo));
    
    DebugLog("roomNo - %d", packet.roomNo);
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(packet.roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    room->finishGame();
    
    LobbyServer::getInstance()->roomMgr->destroyRoom(packet.roomNo);
    
    GameLobbyPacket::FinishGameResPacket sendPacket;
    sendPacket.roomNo = packet.roomNo;
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::gameHandleMovingClientDisconnect(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::MovingClientDisconnectPacket packet;
    
    memcpy(&packet.userNo, body, sizeof(packet.userNo));
    
    if(LobbyServer::getInstance()->userMgr->removeUnconnectedUserByUserNo(packet.userNo) == false)
    {
        ErrorLog("not exist unconnect user - %lld", packet.userNo);
        
        User* user = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
        
        if(user == NULL)
        {
            ErrorLog("??");
            return ;
        }
    }
}



//////////////////////////////////////// chatting recv

void IOManager::chattingReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case LobbyChattingPacket::FIRST_CONNECT_RES:
            chattingHandleFirstConnectRes(connectInfo, body, bodySize);
            break;
            
        case LobbyChattingPacket::ENTER_CLIENT_RES:
            chattingHandleEnterClientRes(connectInfo, body, bodySize);
            break;
            
        case LobbyChattingPacket::ENTER_CLIENT_OK:
            frontHandleEnterClientOk(connectInfo, body, bodySize);
            break;
            
        case LobbyChattingPacket::ENTER_CLIENT_OUT:
            frontHandleEnterClientOut(connectInfo, body, bodySize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::chattingSessionIn(ConnectInfo* connectInfo)
{
    LobbyChattingPacket::FirstConnectReqPacket packet;
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&packet, sizeof(packet));
}


void IOManager::chattingSessionOut(ConnectInfo* connectInfo)
{
    ErrorLog("never chatting session out");
}



void IOManager::chattingHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    LobbyChattingPacket::FirstConnectResPacket packet;
    
    memcpy(packet.clientIp, body, MAX_IP_ADDRESS_LEN);
    body += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.clientPort, body, sizeof(packet.clientPort));
    body += sizeof(packet.clientPort);
    
    DebugLog("chatting ip port : %s %d", packet.clientIp, packet.clientPort);
    
    LobbyServer::getInstance()->serverInfoMgr->addChattingServer(connectInfo, packet.clientIp, packet.clientPort);
}


void IOManager::chattingHandleEnterClientRes(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    LobbyChattingPacket::EnterClientResPacket packet;
    
    memcpy(&packet.userNo, body, sizeof(packet.userNo));
    body += sizeof(packet.userNo);
    
    User* user = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
    
    if(user == NULL || user->getConnectInfo() == NULL)
    {
        DebugLog("already out");
        return ;
    }
    
    ChattingServerInfo* chattingServerInfo = LobbyServer::getInstance()->serverInfoMgr->getChattingServerInfo();
    
    if(chattingServerInfo == NULL)
    {
        ErrorLog("chatting server not exist");
        return ;
    }
    
    ClientLobbyPacket::FirstConnectResPacket sendPacket;
    memcpy(sendPacket.chattingIp, chattingServerInfo->clientIp, MAX_IP_ADDRESS_LEN);
    sendPacket.chattingPort = chattingServerInfo->clientPort;
    
    user->getUserInfo(&sendPacket.userInfo);
    
    user->getFriendList(sendPacket.nickNameInfoWithOnlineList);
    
    if(user->getClanNo() != INVALID_CLAN_NO)
    {
        Clan* clan = LobbyServer::getInstance()->clanMgr->getClanByClanNo(user->getClanNo());
        
        if(clan != NULL)
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
    
    memcpy(pSendBuffer, &sendPacket.chattingIp, MAX_IP_ADDRESS_LEN);
    pSendBuffer += MAX_IP_ADDRESS_LEN;
    
    memcpy(pSendBuffer, &sendPacket.chattingPort, sizeof(sendPacket.chattingPort));
    pSendBuffer += sizeof(sendPacket.chattingPort);
    
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
        memcpy(pSendBuffer, &sendPacket.nickNameInfoWithOnlineList[i].online, sizeof(sendPacket.nickNameInfoWithOnlineList[i].online));
        pSendBuffer += sizeof(sendPacket.nickNameInfoWithOnlineList[i].online);
        
        memcpy(pSendBuffer, &sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen, sizeof(sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen));
        pSendBuffer += sizeof(sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen);
        
        memcpy(pSendBuffer, sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickName, sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen);
        pSendBuffer += sendPacket.nickNameInfoWithOnlineList[i].nickNameInfo.nickNameLen;
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
    
    LobbyServer::getInstance()->network->sendPacket(user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));

}



/////////////////////////////////client recv



void IOManager::clientSessionIn(ConnectInfo* connectInfo)
{
    
    
    
    
}

void IOManager::clientSessionOut(ConnectInfo* connectInfo)
{
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        DebugLog("user not login");
        return;
    }
    else
    {
        if(user->getUserState() == USER_STATE_LOBBY || user->getUserState() == USER_STATE_WAIT_START_IN_LOBBY)
        {
            DebugLog("delete from channel");
            Channel* channel = (Channel*)user->getLocationObject();
            if(channel == NULL)
            {
                ErrorLog("not channel %d", user->getChannelNo());
            }
            else
            {
                if(user->getUserState() == USER_STATE_WAIT_START_IN_LOBBY)
                {
                    if(LobbyServer::getInstance()->quickPlayMgr->removeUser(user) == false)
                    {
                        ErrorLog("quickPlayMgr->removeUser(user)");
                    }
                }
                
                
                if(channel->removeUser(user) < 0)
                {
                    ErrorLog("channel->removeUser(user)");
                }
            }

        }
        else if(user->getUserState() == USER_STATE_ROOM || user->getUserState() == USER_STATE_MOVING_GAME)
        {
            Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(user->getRoomNo());
            
            if(room == NULL)
            {
                ErrorLog("??");
            }
            else
            {
                if(room->getRoomState() == ROOM_STATE_STAND_BY)
                {
                    if(LobbyServer::getInstance()->roomMgr->leaveRoom(user) < 0)
                    {
                        ErrorLog("??");
                    }
                    else
                    {

                    }
                }
                else if(room->getRoomState() == ROOM_STATE_START) // moving game or gaming
                {
//                    DebugLog("go game server");
//                    
//                    if(user->getUserState() == USER_STATE_MOVING_GAME)
//                    {
//                        GameLobbyPacket::StartGameYesPacket packet;
//                        
//                        packet.roomNo = room->getRoomNo();
//                        
//                        LobbyServer::getInstance()->network->sendPacket(LobbyServer::getInstance()->serverInfoMgr->getGameServerInfo()->connectInfo, (char*)&packet, sizeof(packet));
//                    }
//                    else if(user->getUserState() == USER_STATE_GAME)
//                    {
//                        DebugLog("ok");
//                    }
                    DebugLog("eeeeee1");
                    if(LobbyServer::getInstance()->roomMgr->leaveRoom(user) < 0)
                    {
                        ErrorLog("??");
                    }
                    else
                    {
                        
                    }
                }
            }
        }
        else if(user->getUserState() == USER_STATE_GAME)
        {
            if(LobbyServer::getInstance()->roomMgr->leaveRoom(user) < 0)
            {
                ErrorLog("??");
            }
            else
            {
                
            }
            
//            connectInfo->userData = NULL;
//            
//            user->setConnectInfo(NULL);
//            
//            return ;
        }
        else
        {
            // not enter room or channel.. just wait req to enter
        }
    }
    DebugLog("eeeeee2");
    if(LobbyServer::getInstance()->userMgr->removeUser(user) == false)
    {
        ErrorLog("userMgr->removeUser(user)");
    }
    
    connectInfo->userData = NULL;
    
    user->setConnectInfo(NULL);
    
}

void IOManager::clientReceiveData(ConnectInfo* connectInfo, const uint32_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case ClientLobbyPacket::FIRST_CONNECT_REQ:
            clientHandleFirstConnectReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::GET_CHANNEL_LIST_REQ:
            clientHandleGetChannelListReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::MOVE_CHANNEL_REQ:
            clientHandleMoveChannelReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::GET_USER_LIST_REQ:
            clientHandleGetUserListReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::GET_USER_INFO_REQ:
            clientHandleGetUserInfoReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::REQUEST_GAME_REQ:
            clientHandleRequestGameReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::REQUEST_GAME_CANCEL_REQ:
            clientHandleRequestGameCancelReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::RESPONSE_GAME_YES_REQ:
            clientHandleResponceGameYesReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::RESPONSE_GAME_NO_REQ:
            clientHandleResponceGameNoReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::GET_ROOM_LIST_REQ:
            clientHandleGetRoomListReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::CREATE_ROOM_REQ:
            clientHandleCreateRoomReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::ENTER_ROOM_REQ:
            clientHandleEnterRoomReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::READY_REQ:
            clientHandleReadyReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::UNREADY_REQ:
            clientHandleUnReadyReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::CHANGE_TRIBE_REQ:
            clientHandleChangeTribeReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::LEAVE_ROOM_REQ:
            clientHandleLeaveRoomReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::START_GAME_OK:
            clientHandleStartGameOk(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::QUICK_PLAY_REQ:
            clientHandleQuickPlayReq(connectInfo, body, bodySize);
            break;
            
        case ClientLobbyPacket::QUICK_PLAY_OK:
            clientHandleQuickPlayOk(connectInfo, body, bodySize);
            break;

        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("ClientReceiveHandler::HandleFirstConnectReq");
    
    ClientLobbyPacket::FirstConnectReqPacket packet;
    
    memcpy(&packet.sid, body, bodySize);
    
    
    User* user = LobbyServer::getInstance()->userMgr->getUnconnectedUserBySessionId(&packet.sid);
    
    if(user == NULL)
    {
        DebugLog("not exist to recv login info");
        return ;
    }
    
    user->setConnectInfo(connectInfo);
    connectInfo->userData = (void*)user;
    
//    
//    int failReason = LobbyServer::getInstance()->userMgr->addConnectedUser(&packet.sid, connectInfo);
//    if(failReason != SUCCESS)
//    {
//        DebugLog("add user %d", failReason);
//        if(failReason == ALREADY_EXIST_USER)
//        {
//            DebugLog("add user already exist");
//            
//            //TODO dissconnect my other user
//            return ;
//        }
//        else
//        {
//            ErrorLog("??");
//            return ;
//        }
//        
//    }
//
    
    LobbyCachePacket::LoginUserReqPacket sendPacket;
    
    sendPacket.userNo = user->getUserNo();
    
    ConnectInfo* cacheConnectInfo = LobbyServer::getInstance()->serverInfoMgr->getCacheServerInfo()->connectInfo;
    
    LobbyServer::getInstance()->network->sendPacket(cacheConnectInfo, (char*)&sendPacket, sizeof(sendPacket));
    
    
    

    
    
    
    
    
    
    return;
}






void IOManager::clientHandleGetChannelListReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    DebugLog("IOManager::clientHandleGetChannelListReq");
    
    ClientLobbyPacket::GetChannelListResPacket packet;
    packet.cmd = ClientLobbyPacket::GET_CHANNEL_LIST_RES;
    
    int channelCount = LobbyServer::getInstance()->channelMgr->getChannelCount();
    packet.channelCount = channelCount;
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);

    memcpy(pSendBuffer, &packet.channelCount, sizeof(packet.channelCount));
    pSendBuffer += sizeof(packet.channelCount);
    
    
    Channel* channel = NULL;
    for(int i = 0; i < channelCount; i++)
    {
        channel = LobbyServer::getInstance()->channelMgr->getChannelByIndex(i);
        if(channel == NULL)
        {
            ErrorLog("channel error");
            break;
        }
        
        
        packet.channelInfoList[i].channelNo = channel->getChannelNo();
        packet.channelInfoList[i].channelNameLen = channel->getChannelNameLen();
        memcpy(packet.channelInfoList[i].channelName, channel->getChannelName(), packet.channelInfoList[i].channelNameLen);
        

        memcpy(pSendBuffer, &packet.channelInfoList[i].channelNo, sizeof(packet.channelInfoList[i].channelNo));
        pSendBuffer += sizeof(packet.channelInfoList[i].channelNo);
        
        memcpy(pSendBuffer, &packet.channelInfoList[i].channelNameLen, sizeof(packet.channelInfoList[i].channelNameLen));
        pSendBuffer += sizeof(packet.channelInfoList[i].channelNameLen);
        
        memcpy(pSendBuffer, packet.channelInfoList[i].channelName, packet.channelInfoList[i].channelNameLen);
        pSendBuffer += packet.channelInfoList[i].channelNameLen;
    }
    
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
    
    
}

void IOManager::clientHandleMoveChannelReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    DebugLog("ClientReceiveHandler::HandleMoveChannelReq");
    
    ClientLobbyPacket::MoveChannelReqPacket packet;
    
    const char* pBody = body;
    
    memcpy(&packet.channelNo, pBody, sizeof(packet.channelNo));
    pBody += sizeof(packet.channelNo);
    
    
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    
    if(packet.channelNo != INVALID_CHANNEL_NO) // move channel
    {
        if (user->getChannelNo() == packet.channelNo)
        {
            ErrorLog("same channel Move user channel = %d, move channel = %d", user->getChannelNo(), packet.channelNo);
            // session out
            return;
        }
        
        Channel* moveToChannel = LobbyServer::getInstance()->channelMgr->getChannelByChannelNo(packet.channelNo);
        
        ////////////move user other channel
        if(user->getUserLocation() == USER_LOCATION_CHANNEL)
        {
            if(moveToChannel == NULL)
            {
                DebugLog("??");
                // send fail
                return;
            }
            
            if(moveToChannel->isPossibleMove() == false)
            {
                DebugLog("not moving");
                //fail send
                return ;
            }
            
            Channel* moveFromChannel = (Channel*)user->getLocationObject();
            
            if(moveFromChannel == NULL)
            {
                ErrorLog("??");
                // session out
                return;
            }
            

            
            if(moveFromChannel->removeUser(user) != SUCCESS)
            {
                ErrorLog("??");
                //session out
                return ;
            }
        }
        else if(user->getUserLocation() == USER_LOCATION_NO)
        {
            if(moveToChannel == NULL)
            {
                DebugLog("already deleted channel");
                int16_t channelNo = LobbyServer::getInstance()->channelMgr->firstEnterUser(user);
                
                if(channelNo < 0)
                {
                    if(channelNo == FULL_USER_IN_CHANNEL)
                    {
                        DebugLog("user full");
                        //sessionOut
                        return ;
                    }
                    else
                    {
                        ErrorLog("channelNo < 0 %d", channelNo);
                        //sessionOut
                        return ;
                    }
                }
                return;
            }
            
            if(moveToChannel->isPossibleMove() == false)
            {
                DebugLog("not moving");
                //fail send
                return ;
            }
        }
        else
        {
            ErrorLog("??");
            //session out
            return ;
        }
        
        if(moveToChannel->addUser(user) != SUCCESS)
        {
            ErrorLog("??");
            //session out
            return ;
        }
    }
    else // not move, first enter channel
    {
        if(user->getUserLocation() != USER_LOCATION_NO)
        {
            DebugLog("user send fuck message");
            //sessionOut
            return ;
        }
        
        int16_t channelNo = LobbyServer::getInstance()->channelMgr->firstEnterUser(user);
        
        if(channelNo < 0)
        {
            if(channelNo == FULL_USER_IN_CHANNEL)
            {
                DebugLog("user full");
                //sessionOut
                return ;
            }
            else
            {
                ErrorLog("channelNo < 0 %d", channelNo);
                //sessionOut
                return ;
            }
        }
    }
    
//    //////////////////////make packet
//    
//    ClientLobbyPacket::MoveChannelResPacket sendPacket;
//    sendPacket.cmd = ClientLobbyPacket::MOVE_CHANNEL_RES;
//    sendPacket.channelInfo.channelNo = user->getChannelNo();
//    sendPacket.channelInfo.channelNameLen = moveToChannel->getChannelNameLen();
//    memcpy(sendPacket.channelInfo.channelName, moveToChannel->getChannelName(), sendPacket.channelInfo.channelNameLen);
//    
//    
//    
//    
//    
//    
//    //////////////////////////////// send Packet
//    DebugLog("%d",user->getChannelNo());
//    
//    char* pSendBuffer = sendBuffer;
//    
//    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
//    pSendBuffer += sizeof(sendPacket.cmd);
//    
//    memcpy(pSendBuffer, &sendPacket.channelInfo.channelNo, sizeof(sendPacket.channelInfo.channelNo));
//    pSendBuffer += sizeof(sendPacket.channelInfo.channelNo);
//    
//    memcpy(pSendBuffer, &sendPacket.channelInfo.channelNameLen, sizeof(sendPacket.channelInfo.channelNameLen));
//    pSendBuffer += sizeof(sendPacket.channelInfo.channelNameLen);
//    
//    
//    memcpy(pSendBuffer, sendPacket.channelInfo.channelName, sendPacket.channelInfo.channelNameLen);
//    pSendBuffer += sendPacket.channelInfo.channelNameLen;
//    
//    
//    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
}

void IOManager::clientHandleGetUserListReq(ConnectInfo* connectInfo, const char* body, int bodySize) // not need
{
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(user->getUserLocation() != USER_LOCATION_CHANNEL)
    {
        ErrorLog("??");
        return ;
    }
    
    ClientLobbyPacket::GetUserListResPacket packet;
    
    Channel* channel = (Channel*)(user->getLocationObject());
    
    std::list<User*> userList = channel->getUserList();
    
    std::list<User*>::iterator itr;
    
    int i = 0;
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        User* otherUser = *itr;
        packet.nickNameInfoList[i].nickNameLen = otherUser->getNickNameLen();
        memcpy(packet.nickNameInfoList[i].nickName, otherUser->getNickName(), otherUser->getNickNameLen());
        
        i++;
    }
    
    packet.userCount = i;
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);
    
    memcpy(pSendBuffer, &packet.userCount, sizeof(packet.userCount));
    pSendBuffer += sizeof(packet.userCount);
    
    for(i = 0; i < packet.userCount; i++)
    {
        memcpy(pSendBuffer, &packet.nickNameInfoList[i].nickNameLen, sizeof(packet.nickNameInfoList[i].nickNameLen));
        pSendBuffer += sizeof(packet.nickNameInfoList[i].nickNameLen);
        
        memcpy(pSendBuffer, packet.nickNameInfoList[i].nickName, packet.nickNameInfoList[i].nickNameLen);
        pSendBuffer += packet.nickNameInfoList[i].nickNameLen;
    }
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
}

void IOManager::clientHandleGetUserInfoReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    ClientLobbyPacket::GetUserInfoReqPacket packet;
    
    memcpy(&packet.nickNameInfo.nickNameLen, body, sizeof(packet.nickNameInfo.nickNameLen));
    body += sizeof(packet.nickNameInfo.nickNameLen);
    
    memcpy(packet.nickNameInfo.nickName, body, packet.nickNameInfo.nickNameLen);
    body += packet.nickNameInfo.nickNameLen;
    
    
    User* otherUser = LobbyServer::getInstance()->userMgr->getUserByNickNameInfo(&packet.nickNameInfo);
    
    if(otherUser == NULL)
    {
        DebugLog("not exist user");
        return ;
    }
    
    ClientLobbyPacket::GetUserInfoResPacket sendPacket;
    memcpy(&sendPacket.userInfo, otherUser->getUserInfo(), sizeof(UserInfo));
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::clientHandleRequestGameReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
//    User* user = (User*)connectInfo->userData;
//    
//    if(user == NULL)
//    {
//        ErrorLog("??");
//        return ;
//    }
//    
//    if(user->getUserState() != USER_STATE_LOBBY)
//    {
//        DebugLog("not lobby");
//        return ;
//    }
//    
//    
//    ClientLobbyPacket::RequestGameReqPacket packet;
//    memcpy(&packet.userNo, body, sizeof(packet.userNo));
//    
//    User* otherUser = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
//    
//    if(otherUser == NULL || otherUser->getUserLocation() != USER_LOCATION_CHANNEL || otherUser->getConnectInfo() == NULL)
//    {
//        DebugLog("not exist user");
//        return ;
//    }
//    
//    ClientLobbyPacket::RequestGameNotifyPacket notifyPacket;
//    notifyPacket.userNo = user->getUserNo();
//    
//    LobbyServer::getInstance()->network->sendPacket(otherUser->getConnectInfo(), (char*)&notifyPacket, sizeof(notifyPacket));
//    
//    user->setRequestGameUserNo(otherUser->getUserNo());
//    
//    ClientLobbyPacket::RequestGameResPacket sendPacket;
//    
//    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::clientHandleRequestGameCancelReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
//    User* user = (User*)connectInfo->userData;
//    
//    if(user == NULL)
//    {
//        ErrorLog("??");
//        return ;
//    }
//    
//    if(user->getUserState() != USER_STATE_LOBBY)
//    {
//        DebugLog("not waiting state");
//        return ;
//    }
//    
//    user->setRequestGameUserNo(INVALID_USER_NO);
//    
//    ClientLobbyPacket::RequestGameCancelResPacket sendPacket;
//    
//    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::clientHandleResponceGameYesReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
//    User* user = (User*)connectInfo->userData;
//    
//    if(user == NULL)
//    {
//        ErrorLog("??");
//        return ;
//    }
//    
//    ClientLobbyPacket::ResponseGameYesReqPacket packet;
//    
//    memcpy(&packet.userNo, body, sizeof(packet.userNo));
//    
//    User* otherUser = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
//    
//    if(otherUser == NULL)
//    {
//        DebugLog("already out");
//        return ;
//    }
//    
//    if(otherUser->getRequestGameUserNo() != user->getUserNo() || otherUser->getUserState() != USER_STATE_LOBBY)
//    {
//        DebugLog("already cancel");
//        return ;
//    }
//    
//    
//    ////////////////start game
//    
//    RoomInfo roomInfo;
//    roomInfo.roomNameLen = 0;
//    
//    user->setRequestGameUserNo(INVALID_USER_NO);
//    
//    Room* room = LobbyServer::getInstance()->roomMgr->createRoom(&roomInfo, otherUser, 0);
//    
//    if(room == NULL)
//    {
//        ErrorLog("??");
//        return ;
//    }
//    
//    otherUser->setRequestGameUserNo(INVALID_USER_NO);
//    
//    if(room->joinUserInRoom(user) != SUCCESS)
//    {
//        ErrorLog("??");
//        return ;
//    }
//    
//    room->startGame();
}

void IOManager::clientHandleResponceGameNoReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
//    User* user = (User*)connectInfo->userData;
//    
//    if(user == NULL)
//    {
//        ErrorLog("??");
//        return ;
//    }
//    
//    ClientLobbyPacket::ResponseGameNoReqPacket packet;
//    
//    memcpy(&packet.userNo, body, sizeof(packet.userNo));
//    
//    User* otherUser = LobbyServer::getInstance()->userMgr->getUserByUserNo(packet.userNo);
//    
//    if(otherUser == NULL || otherUser->getUserState() != USER_STATE_LOBBY)
//    {
//        DebugLog("already out");
//        return ;
//    }
//    
//    if(otherUser->getRequestGameUserNo() != user->getUserNo())
//    {
//        DebugLog("already cancel");
//        return ;
//    }
//    
//    ClientLobbyPacket::ResponseGameNoNotifyPacket notifyPacket;
//
//    LobbyServer::getInstance()->network->sendPacket(otherUser->getConnectInfo(), (char*)&notifyPacket, sizeof(notifyPacket));
}


void IOManager::clientHandleGetRoomListReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("IOManager::clientHandleGetRoomListReq");
    
    ClientLobbyPacket::GetRoomListResPacket packet;
    packet.cmd = ClientLobbyPacket::GET_ROOM_LIST_RES;
    
    int roomCount = LobbyServer::getInstance()->roomMgr->getRoomCount();
    packet.roomCount = roomCount;
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);
    
    memcpy(pSendBuffer, &packet.roomCount, sizeof(packet.roomCount));
    pSendBuffer += sizeof(packet.roomCount);
    
    
    Room* room = NULL;
    for(int i = 0; i < roomCount; i++)
    {
        room = LobbyServer::getInstance()->roomMgr->getRoomByIndex(i);
        if(room == NULL)
        {
            ErrorLog("channel error");
            break;
        }
        
        
        packet.roomInfoList[i].roomNo = room->getRoomNo();
        packet.roomInfoList[i].roomNameLen = room->getRoomNameLen();
        memcpy(packet.roomInfoList[i].roomName, room->getRoomName(), packet.roomInfoList[i].roomNameLen);
        
        
        memcpy(pSendBuffer, &packet.roomInfoList[i].roomNo, sizeof(packet.roomInfoList[i].roomNo));
        pSendBuffer += sizeof(packet.roomInfoList[i].roomNo);
        
        memcpy(pSendBuffer, &packet.roomInfoList[i].roomNameLen, sizeof(packet.roomInfoList[i].roomNameLen));
        pSendBuffer += sizeof(packet.roomInfoList[i].roomNameLen);
        
        memcpy(pSendBuffer, packet.roomInfoList[i].roomName, packet.roomInfoList[i].roomNameLen);
        pSendBuffer += packet.roomInfoList[i].roomNameLen;
    }
    
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
    
}


void IOManager::clientHandleCreateRoomReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("ClientReceiveHandler::clientHandleCreateRoomReq");
    
    ClientLobbyPacket::CreateRoomReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (sizeof(packet) < bodySize)
    {
        ErrorLog("??");
        return;
    }
    
    const char* pBody = body;
    
    memcpy(&packet.roomInfo.roomNo, pBody, sizeof(packet.roomInfo.roomNo));
    pBody += sizeof(packet.roomInfo.roomNo);
    
    memcpy(&packet.roomInfo.roomNameLen, pBody, sizeof(packet.roomInfo.roomNameLen));
    pBody += sizeof(packet.roomInfo.roomNameLen);
    
    memcpy(&packet.roomInfo.roomName, pBody, packet.roomInfo.roomNameLen);
    pBody += packet.roomInfo.roomNameLen;
    
    memcpy(&packet.mapType, pBody, sizeof(packet.mapType));
    pBody += sizeof(packet.mapType);
    
    
    
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    
    
    ////////////////////////create Room
    
    RoomInfo roomInfo;
    roomInfo.roomNo = 0;
    roomInfo.roomNameLen = packet.roomInfo.roomNameLen;
    memcpy(roomInfo.roomName, packet.roomInfo.roomName, roomInfo.roomNameLen);
    
    
    Room* room = LobbyServer::getInstance()->roomMgr->createRoom(&roomInfo, user, packet.mapType);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    user->setRoomNo(room->getRoomNo());
    
    
    
    
    ////////////leave channel
    
    
    Channel* channel = LobbyServer::getInstance()->channelMgr->getChannelByChannelNo(user->getChannelNo());
    
    if(channel == NULL)
    {
        ErrorLog("??");
        // send fail
        return;
    }
    
    
    if(channel->removeUser(user) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    user->setChannelNo(INVALID_CHANNEL_NO);
    
    
    
    //////////////////////make packet
    
    ClientLobbyPacket::CreateRoomResPacket sendPacket;
    sendPacket.cmd = ClientLobbyPacket::CREATE_ROOM_RES;
    sendPacket.roomDetailInfo.roomNo = room->getRoomNo();
    sendPacket.roomDetailInfo.roomNameLen = room->getRoomNameLen();
    memcpy(sendPacket.roomDetailInfo.roomName, room->getRoomName(), sendPacket.roomDetailInfo.roomNameLen);
    sendPacket.roomDetailInfo.mapType = room->getMapType();
    sendPacket.roomDetailInfo.masterIndex = room->getMasterIndex();
    
    
    const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        sendPacket.roomDetailInfo.roomSlotList[i].isOpen = roomSlotList[i].isOpen;
        if(roomSlotList[i].isOpen == 1)
        {
            sendPacket.roomDetailInfo.roomSlotList[i].isExistUser = roomSlotList[i].isExistUser;
            if(roomSlotList[i].isExistUser == 1)
            {
                sendPacket.roomDetailInfo.roomSlotList[i].isReady = roomSlotList[i].isReady;
                sendPacket.roomDetailInfo.roomSlotList[i].tribe = roomSlotList[i].tribe;
                sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen = roomSlotList[i].user->getNickNameLen();
                memcpy(sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickName, roomSlotList[i].user->getNickName(), roomSlotList[i].user->getNickNameLen());
            }
        }
    }
    
    
    
    
    
    //////////////////////////////// send Packet
    DebugLog("%d",user->getChannelNo());
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomNo, sizeof(sendPacket.roomDetailInfo.roomNo));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomNo);
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomNameLen, sizeof(sendPacket.roomDetailInfo.roomNameLen));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomNameLen);
    
    memcpy(pSendBuffer, sendPacket.roomDetailInfo.roomName, sendPacket.roomDetailInfo.roomNameLen);
    pSendBuffer += sendPacket.roomDetailInfo.roomNameLen;
    
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.mapType, sizeof(sendPacket.roomDetailInfo.mapType));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.mapType);
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.masterIndex, sizeof(sendPacket.roomDetailInfo.masterIndex));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.masterIndex);
    
    
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].isOpen, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isOpen));
        pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isOpen);
        
        if(sendPacket.roomDetailInfo.roomSlotList[i].isOpen == 1)
        {
            memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].isExistUser, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isExistUser));
            pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isExistUser);
            
            if(sendPacket.roomDetailInfo.roomSlotList[i].isExistUser == 1)
            {
                memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].isReady, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isReady));
                pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isReady);
                
                memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].tribe, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].tribe));
                pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].tribe);
                
                memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen));
                pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen);
                
                
                memcpy(pSendBuffer, sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickName, sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen);
                pSendBuffer += sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen;
            }
        }
    }
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));

}


void IOManager::clientHandleEnterRoomReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("ClientReceiveHandler::clientHandleEnterRoomReq");
    
    ClientLobbyPacket::EnterRoomReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (sizeof(packet) < bodySize)
    {
        ErrorLog("??");
        return;
    }
    
    const char* pBody = body;
    
    memcpy(&packet.roomNo, pBody, sizeof(packet.roomNo));
    pBody += sizeof(packet.roomNo);
    
    
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    
    ////////////////////////enter Room
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(packet.roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(room->joinUserInRoom(user) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    user->setRoomNo(room->getRoomNo());
    
    
    ////////////leave channel
    
    
    Channel* channel = LobbyServer::getInstance()->channelMgr->getChannelByChannelNo(user->getChannelNo());
    
    if(channel == NULL)
    {
        ErrorLog("??");
        // send fail
        return;
    }
    
    
    if(channel->removeUser(user) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    user->setChannelNo(INVALID_CHANNEL_NO);

    
    
    
    
    
    
    
    //////////////////////make packet
    
    ClientLobbyPacket::EnterRoomResPacket sendPacket;
    sendPacket.cmd = ClientLobbyPacket::ENTER_ROOM_RES;
    sendPacket.myIndex = room->getIndexByUser(user);
    sendPacket.roomDetailInfo.roomNo = room->getRoomNo();
    sendPacket.roomDetailInfo.roomNameLen = room->getRoomNameLen();
    memcpy(sendPacket.roomDetailInfo.roomName, room->getRoomName(), sendPacket.roomDetailInfo.roomNameLen);
    sendPacket.roomDetailInfo.mapType = room->getMapType();
    sendPacket.roomDetailInfo.masterIndex = room->getMasterIndex();
    
    
    const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        sendPacket.roomDetailInfo.roomSlotList[i].isOpen = roomSlotList[i].isOpen;
        if(roomSlotList[i].isOpen == 1)
        {
            sendPacket.roomDetailInfo.roomSlotList[i].isExistUser = roomSlotList[i].isExistUser;
            if(roomSlotList[i].isExistUser == 1)
            {
                sendPacket.roomDetailInfo.roomSlotList[i].isReady = roomSlotList[i].isReady;
                sendPacket.roomDetailInfo.roomSlotList[i].tribe = roomSlotList[i].tribe;
                sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen = roomSlotList[i].user->getNickNameLen();
                memcpy(sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickName, roomSlotList[i].user->getNickName(), roomSlotList[i].user->getNickNameLen());
            }
        }
    }
    
    
    ClientLobbyPacket::EnterRoomNotifyPacket notifyPacket;
    notifyPacket.cmd = ClientLobbyPacket::ENTER_ROOM_NOTIFY;
    notifyPacket.enterIndex = sendPacket.myIndex;
    
    notifyPacket.roomSlot.isOpen = roomSlotList[notifyPacket.enterIndex].isOpen;
    if(roomSlotList[notifyPacket.enterIndex].isOpen == 1)
    {
        notifyPacket.roomSlot.isExistUser = roomSlotList[notifyPacket.enterIndex].isExistUser;
        if(roomSlotList[notifyPacket.enterIndex].isExistUser == 1)
        {
            notifyPacket.roomSlot.isReady = roomSlotList[notifyPacket.enterIndex].isReady;
            notifyPacket.roomSlot.tribe = roomSlotList[notifyPacket.enterIndex].tribe;
            notifyPacket.roomSlot.nickNameInfo.nickNameLen = roomSlotList[notifyPacket.enterIndex].user->getNickNameLen();
            memcpy(notifyPacket.roomSlot.nickNameInfo.nickName, roomSlotList[notifyPacket.enterIndex].user->getNickName(), roomSlotList[notifyPacket.enterIndex].user->getNickNameLen());
        }
    }
    
    
    //////////////////////////////// send Packet
    DebugLog("%d",user->getChannelNo());
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.myIndex, sizeof(sendPacket.myIndex));
    pSendBuffer += sizeof(sendPacket.myIndex);
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomNo, sizeof(sendPacket.roomDetailInfo.roomNo));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomNo);
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomNameLen, sizeof(sendPacket.roomDetailInfo.roomNameLen));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomNameLen);
    
    memcpy(pSendBuffer, sendPacket.roomDetailInfo.roomName, sendPacket.roomDetailInfo.roomNameLen);
    pSendBuffer += sendPacket.roomDetailInfo.roomNameLen;
    
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.mapType, sizeof(sendPacket.roomDetailInfo.mapType));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.mapType);
    
    memcpy(pSendBuffer, &sendPacket.roomDetailInfo.masterIndex, sizeof(sendPacket.roomDetailInfo.masterIndex));
    pSendBuffer += sizeof(sendPacket.roomDetailInfo.masterIndex);
    
    
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].isOpen, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isOpen));
        pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isOpen);
        
        if(sendPacket.roomDetailInfo.roomSlotList[i].isOpen == 1)
        {
            memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].isExistUser, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isExistUser));
            pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isExistUser);
            
            if(sendPacket.roomDetailInfo.roomSlotList[i].isExistUser == 1)
            {
                memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].isReady, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isReady));
                pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].isReady);
                
                memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].tribe, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].tribe));
                pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].tribe);
                
                memcpy(pSendBuffer, &sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen, sizeof(sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen));
                pSendBuffer += sizeof(sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen);
                
                
                memcpy(pSendBuffer, sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickName, sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen);
                pSendBuffer += sendPacket.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen;
            }
        }
    }
    
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));

    
    
    
    
    
    pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pSendBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pSendBuffer, &notifyPacket.enterIndex, sizeof(notifyPacket.enterIndex));
    pSendBuffer += sizeof(notifyPacket.enterIndex);
    
    memcpy(pSendBuffer, &notifyPacket.roomSlot.isOpen, sizeof(notifyPacket.roomSlot.isOpen));
    pSendBuffer += sizeof(notifyPacket.roomSlot.isOpen);
    
    if(notifyPacket.roomSlot.isOpen == 1)
    {
        memcpy(pSendBuffer, &notifyPacket.roomSlot.isExistUser, sizeof(notifyPacket.roomSlot.isExistUser));
        pSendBuffer += sizeof(notifyPacket.roomSlot.isExistUser);
        
        if(notifyPacket.roomSlot.isExistUser == 1)
        {
            memcpy(pSendBuffer, &notifyPacket.roomSlot.isReady, sizeof(notifyPacket.roomSlot.isReady));
            pSendBuffer += sizeof(notifyPacket.roomSlot.isReady);
            
            memcpy(pSendBuffer, &notifyPacket.roomSlot.tribe, sizeof(notifyPacket.roomSlot.tribe));
            pSendBuffer += sizeof(notifyPacket.roomSlot.tribe);
            
            memcpy(pSendBuffer, &notifyPacket.roomSlot.nickNameInfo.nickNameLen, sizeof(notifyPacket.roomSlot.nickNameInfo.nickNameLen));
            pSendBuffer += sizeof(notifyPacket.roomSlot.nickNameInfo.nickNameLen);
            
            memcpy(pSendBuffer, notifyPacket.roomSlot.nickNameInfo.nickName, notifyPacket.roomSlot.nickNameInfo.nickNameLen);
            pSendBuffer += notifyPacket.roomSlot.nickNameInfo.nickNameLen;
        }
    }
    
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user != user)
        {
            LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
        }
    }
}


void IOManager::clientHandleReadyReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    int roomNo = user->getRoomNo();
    
    if(roomNo == INVALID_ROOM_NO)
    {
        ErrorLog("invalid roomNo");
        return ;
    }
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(roomNo);
    
    if(room == NULL)
    {
        ErrorLog("?? %d", roomNo);
        return ;
    }
    
    
    if(room->readyUserInRoom(user) == false)
    {
        ErrorLog("??");
        return ;
    }
    
    
    int userIndex = room->getIndexByUser(user);
    int masterIndex = room->getMasterIndex();
    
    if(userIndex != masterIndex)   /////////not master
    {
        //////////////////
        
        ClientLobbyPacket::ReadyResPacket sendPacket;
        sendPacket.cmd = ClientLobbyPacket::READY_RES;
        
        
        ClientLobbyPacket::ReadyNotifyPacket notifyPacket;
        notifyPacket.cmd = ClientLobbyPacket::READY_NOTIFY;
        notifyPacket.readyIndex = room->getIndexByUser(user);
        
        
        //////////////////
        
        
        
        
        char* pSendBuffer = sendBuffer;
        
        memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
        pSendBuffer += sizeof(sendPacket.cmd);
        
        LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
        
        
        
        pSendBuffer = sendBuffer;
        
        memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
        pSendBuffer += sizeof(notifyPacket.cmd);
        
        memcpy(pSendBuffer, &notifyPacket.readyIndex, sizeof(notifyPacket.readyIndex));
        pSendBuffer += sizeof(notifyPacket.readyIndex);
        
        
        const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
        
        for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
        {
            if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user != user)
            {
                LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
            }
        }
        
    }
    else////////// master start game
    {
        const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
        
        GameLobbyPacket::EnterRoomReqPacket packet;
        packet.roomNo = room->getRoomNo();

        int userCount = 0;
        
        for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
        {
            if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
            {
                memcpy(&packet.userInfoWithSessionId[userCount].sessionId, roomSlotList[i].user->getSid(), sizeof(SessionId_t));
                memcpy(&packet.userInfoWithSessionId[userCount].userInfo, roomSlotList[i].user->getUserInfo(), sizeof(UserInfo));
                userCount++;
            }
        }
        
        packet.userCount = userCount;
        
        char* pSendBuffer = sendBuffer;
        
        memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
        pSendBuffer += sizeof(packet.cmd);
        
        memcpy(pSendBuffer, &packet.roomNo, sizeof(packet.roomNo));
        pSendBuffer += sizeof(packet.roomNo);
        
        memcpy(pSendBuffer, &packet.userCount, sizeof(packet.userCount));
        pSendBuffer += sizeof(packet.userCount);
        
        for(int i = 0; i < packet.userCount; i++)
        {
            memcpy(pSendBuffer, &packet.userInfoWithSessionId[i].sessionId, sizeof(SessionId_t));
            pSendBuffer += sizeof(SessionId_t);
        
            memcpy(pSendBuffer, &packet.userInfoWithSessionId[i].userInfo, sizeof(UserInfo));
            pSendBuffer += sizeof(UserInfo);
        }
        
        LobbyServer::getInstance()->network->sendPacket(LobbyServer::getInstance()->serverInfoMgr->getFreeGameServerInfo()->connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
    }
}


void IOManager::clientHandleUnReadyReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    int roomNo = user->getRoomNo();
    
    if(roomNo == INVALID_ROOM_NO)
    {
        ErrorLog("invalid roomNo");
        return ;
    }
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(roomNo);
    
    if(room == NULL)
    {
        ErrorLog("?? %d", roomNo);
        return ;
    }
    
    if(room->unReadyUserInRoom(user) == false)
    {
        ErrorLog("??");
        return ;
    }
    
    
    
    //////////////////
    
    ClientLobbyPacket::UnreadyResPacket sendPacket;
    sendPacket.cmd = ClientLobbyPacket::UNREADY_RES;
    
    
    ClientLobbyPacket::UnreadyNotifyPacket notifyPacket;
    notifyPacket.cmd = ClientLobbyPacket::UNREADY_NOTIFY;
    notifyPacket.unreadyIndex = room->getIndexByUser(user);
    
    
    //////////////////
    
    
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
    
    
    
    pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pSendBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pSendBuffer, &notifyPacket.unreadyIndex, sizeof(notifyPacket.unreadyIndex));
    pSendBuffer += sizeof(notifyPacket.unreadyIndex);
    
    
    const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user != user)
        {
            LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
        }
    }
}


void IOManager::clientHandleChangeTribeReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    ClientLobbyPacket::ChangeTribeReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (sizeof(packet) < bodySize)
    {
        ErrorLog("??");
        return;
    }
    
    const char* pBody = body;
    
    memcpy(&packet.tribe, pBody, sizeof(packet.tribe));
    pBody += sizeof(packet.tribe);
    
    
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    int roomNo = user->getRoomNo();
    
    if(roomNo == INVALID_ROOM_NO)
    {
        ErrorLog("invalid roomNo");
        return ;
    }
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(roomNo);
    
    if(room == NULL)
    {
        ErrorLog("?? %d", roomNo);
        return ;
    }
    
    if(room->changeTribe(user, packet.tribe) == false)
    {
        ErrorLog("??");
        return ;
    }
    
    
    
    //////////////////
    
    ClientLobbyPacket::ChangeTribeResPacket sendPacket;
    sendPacket.cmd = ClientLobbyPacket::CHANGE_TRIBE_RES;
    sendPacket.tribe = packet.tribe;
    
    
    ClientLobbyPacket::ChangeTribeNotifyPacket notifyPacket;
    notifyPacket.cmd = ClientLobbyPacket::CHANGE_TRIBE_NOTIFY;
    notifyPacket.changeTribeIndex = room->getIndexByUser(user);
    notifyPacket.tribe = packet.tribe;
    
    
    //////////////////
    
    
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.tribe, sizeof(sendPacket.tribe));
    pSendBuffer += sizeof(sendPacket.tribe);
    
    
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));
    
    
    
    pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pSendBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pSendBuffer, &notifyPacket.changeTribeIndex, sizeof(notifyPacket.changeTribeIndex));
    pSendBuffer += sizeof(notifyPacket.changeTribeIndex);
    
    memcpy(pSendBuffer, &notifyPacket.tribe, sizeof(notifyPacket.tribe));
    pSendBuffer += sizeof(notifyPacket.tribe);
    
    
    const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1 && roomSlotList[i].user != user)
        {
            LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
        }
    }
}


void IOManager::clientHandleLeaveRoomReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    
    User* user = (User*)connectInfo->userData;
    if (user == NULL)
    {
        ErrorLog("user not exist user");
        //send fail
        return;
    }
    
    
    
    ////////////enter channel
    
    int16_t channelNo = LobbyServer::getInstance()->channelMgr->firstEnterUser(user);
    
    if(channelNo == INVALID_CHANNEL_NO)
    {
        ErrorLog("??");
        // send fail
        return;
    }
    
    Channel* channel = LobbyServer::getInstance()->channelMgr->getChannelByChannelNo(channelNo);
    
    if(channel == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    
    user->setChannelNo(channelNo);
    
    
    
    
    
    ////////////////////////leave Room
    
    int roomNo = user->getRoomNo();

    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    int8_t index = room->getIndexByUser(user);
    
    if(LobbyServer::getInstance()->roomMgr->leaveRoom(user) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(roomNo);
    
    
    user->setRoomNo(INVALID_ROOM_NO);
    
    

    
    
    
    //////////////////////make packet
    
    ClientLobbyPacket::LeaveRoomResPacket sendPacket;
    sendPacket.cmd = ClientLobbyPacket::LEAVE_ROOM_RES;
    sendPacket.channelInfo.channelNo = user->getChannelNo();
    sendPacket.channelInfo.channelNameLen = channel->getChannelNameLen();
    memcpy(sendPacket.channelInfo.channelName, channel->getChannelName(), sendPacket.channelInfo.channelNameLen);
    
    //////////////////////////////// send Packet
    
    DebugLog("%d",user->getChannelNo());
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &sendPacket.cmd, sizeof(sendPacket.cmd));
    pSendBuffer += sizeof(sendPacket.cmd);
    
    memcpy(pSendBuffer, &sendPacket.channelInfo.channelNo, sizeof(sendPacket.channelInfo.channelNo));
    pSendBuffer += sizeof(sendPacket.channelInfo.channelNo);
    
    memcpy(pSendBuffer, &sendPacket.channelInfo.channelNameLen, sizeof(sendPacket.channelInfo.channelNameLen));
    pSendBuffer += sizeof(sendPacket.channelInfo.channelNameLen);
    
    
    memcpy(pSendBuffer, sendPacket.channelInfo.channelName, sendPacket.channelInfo.channelNameLen);
    pSendBuffer += sendPacket.channelInfo.channelNameLen;
    
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, sendBuffer, (int)(pSendBuffer - sendBuffer));

    
    
    ////////////////////send noti
    
    if(room == NULL) //room destroy
    {
        
    }
    else
    {
        clientSendLeaveRoomNotify(room, index);
    }
}

void IOManager::clientSendLeaveRoomNotify(Room* room, int8_t leaveIndex)
{
    ClientLobbyPacket::LeaveRoomNotifyPacket notifyPacket;
    notifyPacket.leaveIndex = leaveIndex;
    
    const RoomSlotInfo* roomSlotList = room->getRoomSlotList();
    
    notifyPacket.roomSlot.isOpen = roomSlotList[leaveIndex].isOpen;
    if(roomSlotList[leaveIndex].isOpen == 1)
    {
        notifyPacket.roomSlot.isExistUser = roomSlotList[leaveIndex].isExistUser;
        if(roomSlotList[leaveIndex].isExistUser == 1)
        {
            ErrorLog("??");
            
            notifyPacket.roomSlot.isReady = roomSlotList[leaveIndex].isReady;
            notifyPacket.roomSlot.tribe = roomSlotList[leaveIndex].tribe;
            notifyPacket.roomSlot.nickNameInfo.nickNameLen = roomSlotList[leaveIndex].user->getNickNameLen();
            memcpy(notifyPacket.roomSlot.nickNameInfo.nickName, roomSlotList[leaveIndex].user->getNickName(), roomSlotList[leaveIndex].user->getNickNameLen());
        }
    }
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &notifyPacket.cmd, sizeof(notifyPacket.cmd));
    pSendBuffer += sizeof(notifyPacket.cmd);
    
    memcpy(pSendBuffer, &notifyPacket.leaveIndex, sizeof(notifyPacket.leaveIndex));
    pSendBuffer += sizeof(notifyPacket.leaveIndex);
    
    memcpy(pSendBuffer, &notifyPacket.roomSlot.isOpen, sizeof(notifyPacket.roomSlot.isOpen));
    pSendBuffer += sizeof(notifyPacket.roomSlot.isOpen);
    
    if(notifyPacket.roomSlot.isOpen == 1)
    {
        memcpy(pSendBuffer, &notifyPacket.roomSlot.isExistUser, sizeof(notifyPacket.roomSlot.isExistUser));
        pSendBuffer += sizeof(notifyPacket.roomSlot.isExistUser);
        
        if(notifyPacket.roomSlot.isExistUser == 1)
        {
            memcpy(pSendBuffer, &notifyPacket.roomSlot.isReady, sizeof(notifyPacket.roomSlot.isReady));
            pSendBuffer += sizeof(notifyPacket.roomSlot.isReady);
            
            memcpy(pSendBuffer, &notifyPacket.roomSlot.tribe, sizeof(notifyPacket.roomSlot.tribe));
            pSendBuffer += sizeof(notifyPacket.roomSlot.tribe);
            
            memcpy(pSendBuffer, &notifyPacket.roomSlot.nickNameInfo.nickNameLen, sizeof(notifyPacket.roomSlot.nickNameInfo.nickNameLen));
            pSendBuffer += sizeof(notifyPacket.roomSlot.nickNameInfo.nickNameLen);
            
            memcpy(pSendBuffer, notifyPacket.roomSlot.nickNameInfo.nickName, notifyPacket.roomSlot.nickNameInfo.nickNameLen);
            pSendBuffer += notifyPacket.roomSlot.nickNameInfo.nickNameLen;
        }
    }
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        if(roomSlotList[i].isOpen == 1 && roomSlotList[i].isExistUser == 1)
        {
            LobbyServer::getInstance()->network->sendPacket(roomSlotList[i].user->getConnectInfo(), sendBuffer, (int)(pSendBuffer - sendBuffer));
        }
    }
}


void IOManager::clientHandleStartGameOk(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    ClientLobbyPacket::StartGameOkPacket packet;
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(user->getUserState() != USER_STATE_MOVING_GAME)
    {
        ErrorLog("??");
        return ;
    }
    
    user->setUserState(USER_STATE_GAME);
    
    
    Room* room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(user->getRoomNo());
    
    DebugLog("room No%d", user->getRoomNo());
    
    if(room == NULL)
    {
        ErrorLog("roomNo = %d", user->getRoomNo());
        return ;
    }
    
    if(room->isPossibleToStart())
    {
        GameLobbyPacket::StartGameYesPacket packet;
        
        packet.roomNo = room->getRoomNo();
        
        GameServerInfo* gameServerInfo = LobbyServer::getInstance()->serverInfoMgr->getGameServerInfo(room->getGameServerNo());
        
        if(gameServerInfo == NULL)
        {
            ErrorLog("??");
            return ;
        }
        
        LobbyServer::getInstance()->network->sendPacket(gameServerInfo->connectInfo, (char*)&packet, sizeof(packet));
    }
    else
    {
        DebugLog("wait");
    }
}

void IOManager::clientHandleQuickPlayReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    ClientLobbyPacket::QuickPlayReqPacket packet;
    
    memcpy(&packet.tribe, body, sizeof(packet.tribe));
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(user->getUserState() == USER_STATE_WAIT_START_IN_LOBBY)
    {
        return ;
    }
    
    LobbyServer::getInstance()->quickPlayMgr->addUser(user, packet.tribe);
    
    user->setUserState(USER_STATE_WAIT_START_IN_LOBBY);
    
    ClientLobbyPacket::QuickPlayResPacket sendPacket;
    
    LobbyServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::clientHandleQuickPlayOk(ConnectInfo* connectInfo, const char* body, const int bodySize) //?
{
    DebugLog("qOK");
    
}



