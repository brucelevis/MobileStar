#include "IOManager.h"
#include "Log.h"
#include "Network.h"
#include "User.h"
#include "LObject.h"
#include "ChattingDefine.h"
#include "ChattingServer.h"
#include "ChattingData.h"

#include "LobbyChattingPacket.h"
#include "ClientChattingPacket.h"


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
        case SERVER_MODULE_FRONT_SERVER:
//            frontSessionIn(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionIn(connectInfo);
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
        case SERVER_MODULE_FRONT_SERVER:
//            frontSessionOut(connectInfo);
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbySessionOut(connectInfo);
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
        case SERVER_MODULE_FRONT_SERVER:
//            frontReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
            break;
        case SERVER_MODULE_LOBBY_SERVER:
            lobbyReceiveData(connectInfo, cmd, pData, dataSize - sizeof(command_t));
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


//////////////////////////////////////// lobby recv

void IOManager::lobbyReceiveData(ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case LobbyChattingPacket::FIRST_CONNECT_REQ:
            lobbyHandleFirstConnectReq(connectInfo, body, bodySize);
            break;

        case LobbyChattingPacket::ENTER_CLIENT_REQ:
            lobbyHandleEnterClientReq(connectInfo, body, bodySize);
            break;

        case LobbyChattingPacket::ENTER_CLIENT_OK:
            lobbyHandleEnterClientOk(connectInfo, body, bodySize);
            break;

        case LobbyChattingPacket::ENTER_CLIENT_OUT:
            lobbyHandleEnterClientOut(connectInfo, body, bodySize);
            break;

        default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::lobbySessionIn(ConnectInfo* connectInfo)
{
    
}


void IOManager::lobbySessionOut(ConnectInfo* connectInfo)
{

}



void IOManager::lobbyHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    LobbyChattingPacket::FirstConnectResPacket packet;

    memcpy(packet.clientIp, ChattingServer::getInstance()->chattingData->getNetworkInfo(SERVER_MODULE_CLIENT)->ip, MAX_IP_ADDRESS_LEN);
    packet.clientPort = ChattingServer::getInstance()->chattingData->getNetworkInfo(SERVER_MODULE_CLIENT)->port;

    DebugLog("%s %d", packet.clientIp, packet.clientPort);
    ChattingServer::getInstance()->network->sendPacket(connectInfo, (char*)&packet, sizeof(packet));
}


void IOManager::lobbyHandleEnterClientReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    LobbyChattingPacket::EnterClientReqPacket packet;

    const char* pBody = body;

    memcpy(&packet.sid, pBody, sizeof(packet.sid));
    pBody += sizeof(packet.sid);

    memcpy(&packet.userNo, pBody, sizeof(packet.userNo));
    pBody += sizeof(packet.userNo);

    memcpy(&packet.nickNameInfo.nickNameLen, pBody, sizeof(packet.nickNameInfo.nickNameLen));
    pBody += sizeof(packet.nickNameInfo.nickNameLen);

    memcpy(packet.nickNameInfo.nickName, pBody, packet.nickNameInfo.nickNameLen);
    pBody += packet.nickNameInfo.nickNameLen;
    
    
    if(ChattingServer::getInstance()->userMgr->addUnconnectedUser(&packet.nickNameInfo, &packet.sid) != SUCCESS)
    {
        ErrorLog("??");
        return ;
    }



    /////////////// sendPacket

    LobbyChattingPacket::EnterClientResPacket sendPacket;

    sendPacket.userNo = packet.userNo;
    
    ChattingServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}


void IOManager::lobbyHandleEnterClientOk(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    
}


void IOManager::lobbyHandleEnterClientOut(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    LobbyChattingPacket::EnterClientOutPacket packet;
    
    const char* pBody = body;
    
    memcpy(&packet.nickNameInfo.nickNameLen, pBody, sizeof(packet.nickNameInfo.nickNameLen));
    pBody += sizeof(packet.nickNameInfo.nickNameLen);
    
    memcpy(packet.nickNameInfo.nickName, pBody, packet.nickNameInfo.nickNameLen);
    pBody += packet.nickNameInfo.nickNameLen;
    
    
    if(ChattingServer::getInstance()->userMgr->removeUnconnectedUserByNickNameInfo(&packet.nickNameInfo) == false)
    {
        User* user = ChattingServer::getInstance()->userMgr->getUserByNickNameInfo(&packet.nickNameInfo);
        
        if(user == NULL)
        {
            ErrorLog("??");
            return ;
        }
        
        ///////////// send packet
        
        ClientChattingPacket::FirstConnectOutPacket sendPacket;
        
        ChattingServer::getInstance()->network->sendPacket(user->getConnectInfo(), (char*)&sendPacket, sizeof(sendPacket));
    }
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
        if(user->getLocationObject() != NULL)
        {
            LObject* lo = (LObject*)user->getLocationObject();
            
            if(lo->leaveUser(user) == false)
            {
                ErrorLog("??");
                return ;
            }
        }
        
        if(ChattingServer::getInstance()->userMgr->removeUser(user) == false)
        {
            ErrorLog("userMgr->removeUser(user)");
        }
        
        connectInfo->userData = NULL;
    }
}

void IOManager::clientReceiveData(ConnectInfo* connectInfo, const uint32_t cmd, const char* body, const int bodySize)
{
    switch(cmd)
    {
        case ClientChattingPacket::FIRST_CONNECT_REQ:
            clientHandleFirstConnectReq(connectInfo, body, bodySize);
            break;
            
        case ClientChattingPacket::MOVE_LOCATION_REQ:
            clientHandleMoveLocationReq(connectInfo, body, bodySize);
            break;
        
        case ClientChattingPacket::SEND_CHATTING_REQ:
            clientHandleSendChattingReq(connectInfo, body, bodySize);
            break;
            
         default:
            ErrorLog("invalid command - type = %d", cmd);
            return ;
    }
}


void IOManager::clientHandleFirstConnectReq(ConnectInfo* connectInfo, const char* body, const int bodySize)
{
    DebugLog("ClientReceiveHandler::HandleFirstConnectReq");
    
    ClientChattingPacket::FirstConnectReqPacket packet;
    
    memcpy(&packet.sessionId, body, bodySize);
    
    
    int failReason = ChattingServer::getInstance()->userMgr->addConnectedUser(&packet.sessionId, connectInfo);
    if(failReason != SUCCESS)
    {
        DebugLog("add user %d", failReason);
        if(failReason == ALREADY_EXIST_USER)
        {
            DebugLog("add user already exist");
            
            //TODO dissconnect my other user
            return ;
        }
        else
        {
            ErrorLog("??");
            return ;
        }
        
    }
    
    
    User* user = (User*)connectInfo->userData;
    if(user == NULL)
    {
        ErrorLog("user not found ");
        return ;
    }
    
    ClientChattingPacket::FirstConnectResPacket sendPacket;

    ChattingServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
    
    return;
}

void IOManager::clientHandleMoveLocationReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    ClientChattingPacket::MoveLocationReqPacket packet;
    
    memcpy(&packet.loNo, body, sizeof(packet.loNo));
    
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    if(user->getLocationObject() != NULL)
    {
        LObject* lo = (LObject*)user->getLocationObject();
        lo->leaveUser(user);
    }
    
    ChattingServer::getInstance()->lObjectMgr->addUser(packet.loNo, user);
    
    ClientChattingPacket::MoveLocationResPacket sendPacket;
    
    ChattingServer::getInstance()->network->sendPacket(connectInfo, (char*)&sendPacket, sizeof(sendPacket));
}

void IOManager::clientHandleSendChattingReq(ConnectInfo* connectInfo, const char* body, int bodySize)
{
    ClientChattingPacket::SendChattingReqPacket packet;
    
    memcpy(&packet.chattingLen, body, sizeof(packet.chattingLen));
    body += sizeof(packet.chattingLen);
    
    memcpy(packet.chatting, body, packet.chattingLen);
    body += packet.chattingLen;
    
    DebugLog("%s %d", packet.chatting, packet.chattingLen);
    
    
    User* user = (User*)connectInfo->userData;
    
    if(user == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    LObject* lo = (LObject*)user->getLocationObject();
    
    if( lo == NULL)
    {
        ErrorLog("??");
        return ;
    }
    
    lo->chattingNotify(user, packet.chatting, packet.chattingLen);
}