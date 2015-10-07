#include "IOManager.h"
#include "Log.h"
#include "Network.h"

#include "GameDefine.h"
#include "GameData.h"
#include "GameServer.h"
#include "Room.h"
#include "User.h"
#include "ServerInfo.h"

#include "FrontReceiveHandler.h"
#include "FrontSendHandler.h"
#include "LobbyReceiveHandler.h"
#include "LobbySendHandler.h"
#include "GameReceiveHandler.h"
#include "GameSendHandler.h"
#include "ClientReceiveHandler.h"
#include "ClientSendHandler.h"

#include "GameFrontPacket.h"
#include "GameLobbyPacket.h"
#include "ClientGamePacket.h"

IOManager::IOManager(FrontSendHandler* _frontSendHandler, LobbySendHandler* _lobbySendHandler, GameSendHandler* _gameSendHandler, ClientSendHandler* _clientSendHandler, FrontReceiveHandler* _frontReceiveHandler, LobbyReceiveHandler* _lobbyReceiveHandler, GameReceiveHandler* _gameReceiveHandler, ClientReceiveHandler* _clientReceiveHandler)
{
    frontReceiveHandler = _frontReceiveHandler;
    frontSendHandler = _frontSendHandler;
    lobbyReceiveHandler = _lobbyReceiveHandler;
    lobbySendHandler = _lobbySendHandler;
    gameReceiveHandler = _gameReceiveHandler;
    gameSendHandler = _gameSendHandler;
    clientReceiveHandler = _clientReceiveHandler;
    clientSendHandler = _clientSendHandler;
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
        case SERVER_MODULE_FRONT_SERVER:
            frontSessionIn(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionIn(connectInfo);
            break;
        case SERVER_MODULE_GAME_SERVER:
 //           gameSessionIn(connectInfo);
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
    GameServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
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
        case SERVER_MODULE_GAME_SERVER:
            gameReceiveHandler->sessionOut(connectInfo);
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
    GameServer::getInstance()->network->finishProcessing(threadPipe.writePipe, connectInfo);
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
    
    command_t
    
    cmd;
    memcpy(&cmd, data, sizeof(command_t));
    pData += sizeof(command_t);
    
    switch (connectInfo->serverModule)
    {
        case SERVER_MODULE_FRONT_SERVER:
            frontReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_GAME_SERVER:
//            gameReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
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




//////////////////////////



void IOManager::frontReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case GameFrontPacket::FIRST_CONNECT_RES:
            frontHandleFirstConnectRes(connectInfo, body, bodySize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::frontSessionIn(ConnectInfo* connectInfo)
{
    GameFrontPacket::FirstConnectReqPacket packet;
    
    GameServer::getInstance()->network->sendPacket(connectInfo, (char*)&packet, sizeof(packet));
}


void IOManager::frontSessionOut(ConnectInfo* connectInfo)
{
    ErrorLog("never disconnect");
}


void IOManager::frontHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    GameFrontPacket::FirstConnectResPacket packet;
    
    const char* pBody = body;
    
    memcpy(packet.lobbyIp, pBody, MAX_IP_ADDRESS_LEN);
    pBody += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.lobbyPort, pBody, sizeof(packet.lobbyPort));
    
    DebugLog("%s %d", packet.lobbyIp, packet.lobbyPort);
    
    ///////////// connect with lobby server
    
    NetworkInfo networkInfo;
    networkInfo.module = SERVER_MODULE_LOBBY_SERVER;
    networkInfo.type = SERVER_TYPE_CLIENT;
    memcpy(networkInfo.ip, packet.lobbyIp, MAX_IP_ADDRESS_LEN);
    networkInfo.port = packet.lobbyPort;
    networkInfo.sendPingInterval = 0;
    networkInfo.disconnectPingCount = 0;
    
    
    if(GameServer::getInstance()->network->AddNetworkInfo(&networkInfo) == false)
    {
        ErrorLog("??");
        return ;
    }
}


/////////////////////// lobby recv

void IOManager::lobbyReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case GameLobbyPacket::FIRST_CONNECT_RES:
            lobbyHandleFirstConnectRes(connectInfo, body, bodySize);
            break;
            
        case GameLobbyPacket::ENTER_ROOM_REQ:
            lobbyHandleEnterRoomReq(connectInfo, body, bodySize);
            break;
            
        case GameLobbyPacket::START_GAME_YES:
            lobbyHandleStartGameYes(connectInfo, body, bodySize);
            break;
            
        case GameLobbyPacket::START_GAME_NO:
            lobbyHandleStartGameNo(connectInfo, body, bodySize);
            break;
            
        case GameLobbyPacket::FINISH_GAME_RES:
            lobbyHandleFinishGameRes(connectInfo, body, bodySize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::lobbySessionIn(ConnectInfo* connectInfo)
{
    GameLobbyPacket::FirstConnectReqPacket packet;
    
    memcpy(packet.clientIp, GameServer::getInstance()->gameData->getNetworkInfo(SERVER_MODULE_CLIENT)->ip, MAX_IP_ADDRESS_LEN);
    packet.clientPort = GameServer::getInstance()->gameData->getNetworkInfo(SERVER_MODULE_CLIENT)->port;
    
    
    GameServer::getInstance()->network->sendPacket(connectInfo, (char*)&packet, sizeof(packet));
}


void IOManager::lobbySessionOut(ConnectInfo* connectInfo)
{
    ErrorLog("never disconnect");
}


void IOManager::lobbyHandleFirstConnectRes(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("lobbyHandleFirstConnectRes");
    GameLobbyPacket::FirstConnectResPacket packet;
    
    const char* pBody = body;
    
    memcpy(packet.clientIp, pBody, MAX_IP_ADDRESS_LEN);
    pBody += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.clientPort, pBody, sizeof(packet.clientPort));
    pBody += sizeof(packet.clientPort);
    
    DebugLog("%s %d", packet.clientIp, packet.clientPort);
    
   if(GameServer::getInstance()->serverInfoMgr->addLobbyServer(connectInfo, packet.clientIp, packet.clientPort) == false)
   {
       ErrorLog("??");
       return ;
   }
}


void IOManager::lobbyHandleEnterRoomReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::EnterRoomReqPacket packet;
    
    const char* pBody = body;

    memcpy(&packet.roomNo, pBody, sizeof(packet.roomNo));
    pBody += sizeof(packet.roomNo);
    
    memcpy(&packet.userCount, pBody, sizeof(packet.userCount));
    pBody += sizeof(packet.userCount);
    
    for(int i = 0; i < packet.userCount; i++)
    {
        memcpy(&packet.userInfoWithSessionId[i].sessionId, pBody, sizeof(SessionId_t));
        pBody += sizeof(SessionId_t);
        
        memcpy(&packet.userInfoWithSessionId[i].userInfo, pBody, sizeof(UserInfo));
        pBody += sizeof(UserInfo);
    }
    
    DebugLog("ok");
    
    RoomInfo roomInfo;
    roomInfo.roomNo = packet.roomNo;
    roomInfo.roomNameLen = 0;
    
    Room* room = GameServer::getInstance()->roomMgr->createRoom(&roomInfo, packet.userCount, packet.userInfoWithSessionId);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    /////////////// sendPacket
    
    
    GameLobbyPacket::EnterRoomResPacket sendPacket;
    
    sendPacket.roomNo = roomInfo.roomNo;
    
    GameServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::lobbyHandleStartGameYes(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::StartGameYesPacket packet;
    
    memcpy(&packet.roomNo, body, sizeof(packet));
    
    Room* room = GameServer::getInstance()->roomMgr->getRoomByRoomNo(packet.roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(room->isPossibleStart())
    {
        ClientGamePacket::StartGameNotifyPacket notifyPacket;
        
        User** userList = room->getUserList();
        
        for(int i = 0; i < room->getUserCount(); i++)
        {
            if(userList[i]->getConnectInfo() != NULL)
            {
                GameServer::getInstance()->network->sendPacket(userList[i]->getConnectInfo(), (char*)&notifyPacket, sizeof(notifyPacket));
            }
        }
    }
    else
    {
        ErrorLog("??");
    }
}


void IOManager::lobbyHandleStartGameNo(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::StartGameNoPacket packet;
    
    memcpy(&packet.roomNo, body, sizeof(packet.roomNo));
    
    Room* room = GameServer::getInstance()->roomMgr->getRoomByRoomNo(packet.roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(room->isPossibleStart())
    {
        ClientGamePacket::StartGameNotifyPacket notifyPacket;
        
        User** userList = room->getUserList();
        
        for(int i = 0; i < room->getUserCount(); i++)
        {
            if(userList[i]->getConnectInfo() != NULL)
            {
                GameServer::getInstance()->network->sendPacket(userList[i]->getConnectInfo(), (char*)&notifyPacket, sizeof(notifyPacket));
            }
        }
    }
}


void IOManager::lobbyHandleFinishGameRes(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    GameLobbyPacket::FinishGameResPacket packet;
    
    memcpy(&packet.roomNo, body, sizeof(packet.roomNo));
    
    Room* room = GameServer::getInstance()->roomMgr->getRoomByRoomNo(packet.roomNo);
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    ClientGamePacket::FinishGameResPacket sendPacket;
    memcpy(sendPacket.ip, GameServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->clientIp, MAX_IP_ADDRESS_LEN);
    sendPacket.port = GameServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->clientPort;
    
    User** userList = room->getUserList();
    
    for(int i = 0; i < room->getUserCount(); i++)
    {
        if(userList[i]->getConnectInfo() != NULL)
        {
            GameServer::getInstance()->network->sendPacket(userList[i]->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
        }
    }
    
    room->finishGame();
    
    GameServer::getInstance()->roomMgr->destroyRoom(room->getRoomNo());
}















///////////////////////////// client recv



void IOManager::clientReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case ClientGamePacket::FIRST_CONNECT_REQ:
            clientHandleFirstConnectReq(connectInfo, body, bodySize);
            break;
            
        case ClientGamePacket::CLIENT_NOTIFY:
            clientHandleClientNotify(connectInfo, body, bodySize);
            break;
            
        case ClientGamePacket::FINISH_GAME_REQ:
            clientHandleFinishGameReq(connectInfo, body, bodySize);
            break;
            
        case ClientGamePacket::MOVE_LOBBY_OK:
            clientHandleMoveLobbyOk(connectInfo, body, bodySize);
            break;
            
        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


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
        
            Room* room = user->getRoom();
            
            if(room == NULL)
            {
                if(user->getUserState() == USER_STATE_FINISH)
                {
                    DebugLog("disconnect when finish");
                    
                    GameLobbyPacket::MovingClientDisconnectPacket packet;
                    packet.userNo = user->getUserNo();
                    
                    GameServer::getInstance()->network->sendPacket(GameServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->connectInfo, (char*)&packet, sizeof(packet));
                }
                else if(user->getUserState() == USER_STATE_GAME)
                {
                    DebugLog("disconnect when game");
                }
                else
                {
                    ErrorLog("??");
                }
                
                if(GameServer::getInstance()->userMgr->removeUser(user) == false)
                {
                    ErrorLog("??");
                }
            }
            else
            {
                if(room->getRoomState() == ROOM_STATE_STAND_BY)
                {
//                    int8_t leaveIndex = room->getIndexByUser(user);
//                    
//                    if(LobbyServer::getInstance()->roomMgr->leaveRoom(user) != SUCCESS)
//                    {
//                        ErrorLog("??");
//                        return ;
//                    }
//                    
//                    room = LobbyServer::getInstance()->roomMgr->getRoomByRoomNo(user->getRoomNo());
//                    
//                    if(room != NULL)
//                    {
//                        clientSendLeaveRoomNotify(room, leaveIndex);
//                    }
//                    
//                    LobbyServer::getInstance()->userMgr->removeUser(user);
//                }
//                else
//                {
//                    DebugLog("go game server");
//                    
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
                }
                else if(room->getRoomState() == ROOM_STATE_START)
                {
                    DebugLog("disconnect when start");
                }
                else
                {
                    
                }
                
                user->setConnectInfo(NULL);
            }
    }
    
    connectInfo->userData = NULL;
}


void IOManager::clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    ClientGamePacket::FirstConnectReqPacket packet;
    memcpy(&packet.sid, body, sizeof(packet.sid));
    
    if(GameServer::getInstance()->userMgr->addConnectedUser(&packet.sid, connectInfo) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    Room* room = user->getRoom();
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    ClientGamePacket::FirstConnectResPacket sendPacket;
    
    GameServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::clientHandleClientNotify(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    Room* room = user->getRoom();
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    const char* wholeBody = body - sizeof(command_t);
    
    User** userList = room->getUserList();
    
    for(int i = 0; i < room->getUserCount(); i++)
    {
        if(userList[i]->getConnectInfo() != NULL && userList[i] != user)
        {
            GameServer::getInstance()->network->sendPacket(userList[i]->getConnectInfo(), wholeBody, bodySize + sizeof(command_t));
        }
    }
}


void IOManager::clientHandleFinishGameReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    Room* room = user->getRoom();
    
    if(room == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    int8_t roomState = room->getRoomState();
    
    if(roomState != ROOM_STATE_FINISH)
    {
        DebugLog("roomState check ok");
        room->setRoomState(ROOM_STATE_FINISH);
        return ;
    }
    
    
    GameLobbyPacket::FinishGameReqPacket sendPacket;
    sendPacket.roomNo = room->getRoomNo();
    
    GameServer::getInstance()->network->sendPacket(GameServer::getInstance()->serverInfoMgr->getLobbyServerInfo()->connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::clientHandleMoveLobbyOk(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(GameServer::getInstance()->userMgr->removeUser(user) == false)
    {
        ErrorLog("??");
        return ;
    }
    
    connectInfo->userData = NULL;
}

