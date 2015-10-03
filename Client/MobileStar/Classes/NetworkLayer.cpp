#include "NetworkLayer.h"
#include "GameDefines.h"
#include "LoginScene.h"
#include "GameClient.h"
#include "LobbyScene.h"
#include "LoginScene.h"
#include "ClientFrontPacket.h"
#include "ClientLobbyPacket.h"
#include "ClientGamePacket.h"
#include "UserListLayer.h"
#include "GameWorld.h"

#include "BasicPacket.h"
USING_NS_CC;

bool NetworkHandler::initialize()
{
    network = new Network();
    if(network->initialize(this) == false)
    {
        return false;
    }
    
    //127.0.0.1
    
    if (network->connectWithServer(SERVER_MODULE_FRONT_SERVER, "1.234.65.217", 20400) == false)
    {
        return false;
    }
        
    return true;
}

void NetworkHandler::Receive(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    const char* pData = data;
    int pDataSize = dataSize;
    
    command_t cmd;
    memcpy(&cmd, pData, sizeof(command_t));
    pData += sizeof(command_t);
    pDataSize -= sizeof(command_t);

    if (connectInfo->serverModule == SERVER_MODULE_FRONT_SERVER)
    {
        switch (cmd)
        {
            case ClientFrontPacket::FIRST_CONNECT_RES:
                frontHandleFirstConnectRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientFrontPacket::ENTER_LOBBY_RES:
                frontHandleEnterLobbyRes(connectInfo, pData, pDataSize);
                break;
                
                
            default:
                printf("type invalid - %d", cmd);
                break;
                
        }
    }
    else if (connectInfo->serverModule == SERVER_MODULE_LOBBY_SERVER)
    {
        switch (cmd)
        {
            case ClientLobbyPacket::FIRST_CONNECT_RES:
                lobbyHandleFirstConnectRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::FIRST_CONNECT_OK:
                lobbyHandleFirstConnectOk(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::GET_CHANNEL_LIST_RES:
                lobbyHandleGetChannelListRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::MOVE_CHANNEL_RES:
                lobbyHandleMoveChannelRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::GET_ROOM_LIST_RES:
                lobbyHandleGetRoomListRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::CREATE_ROOM_RES:
                lobbyHandleCreateRoomRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::ENTER_ROOM_RES:
                lobbyHandleEnterRoomRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::ENTER_ROOM_NOTIFY:
                lobbyHandleEnterRoomNotify(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::READY_RES:
                lobbyHandleReadyRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::READY_NOTIFY:
                lobbyHandleReadyNotify(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::UNREADY_RES:
                lobbyHandleUnReadyRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::UNREADY_NOTIFY:
                lobbyHandleUnReadyNotify(connectInfo, pData, pDataSize);
                break;
                
                
                
            case ClientLobbyPacket::CHANGE_TRIBE_RES:
                lobbyHandleChangeTribeRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::CHANGE_TRIBE_NOTIFY:
                lobbyHandleChangeTribeNotify(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::LEAVE_ROOM_RES:
                lobbyHandleLeaveRoomRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::LEAVE_ROOM_NOTIFY:
                lobbyHandleLeaveRoomNotify(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::START_GAME_NOTIFY:
                lobbyHandleStartGameNotify(connectInfo, pData, pDataSize);
                break;

            case ClientLobbyPacket::QUICK_PLAY_RES:
                lobbyHandleQuickPlayRes(connectInfo, pData, pDataSize);
                break;

            default:
                printf("type invalid - %d", cmd);
                break;
                
        }
    }
    else if(connectInfo->serverModule == SERVER_MODULE_GAME_SERVER)
    {
        switch (cmd)
        {
            case ClientGamePacket::FIRST_CONNECT_RES:
                gameHandleFirstConnectRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientGamePacket::START_GAME_NOTIFY:
                gameHandleStartGameNotify(connectInfo, pData, pDataSize);
                break;
                
            case ClientGamePacket::CLIENT_NOTIFY:
                gameHandleClientNotify(connectInfo, pData, pDataSize);
                break;
                
                
                
                
            default:
                printf("type invalid - %d", cmd);
                break;
                
        }
    }
}

void NetworkHandler::frontHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::frontHandleFirstConnectRes");
    
    ///////////////////TODO. draw waitting bar for user
    
    
    ///////////////////send enter lobby req

    frontSendEnterLobbyReq();
}


void NetworkHandler::frontHandleEnterLobbyRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
    printf("NetworkHandler::frontHandleEnterLobbyRes");

    
    ClientFrontPacket::EnterLobbyResPacket packet;
    
    const char* pData = data;
    
    memcpy(packet.ip, pData, MAX_IP_ADDRESS_LEN);
    pData += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.port, pData, sizeof(packet.port));
    pData += sizeof(packet.port);
    
    printf("%s %d", packet.ip, packet.port);
    
    if(network->connectWithServer(SERVER_MODULE_LOBBY_SERVER, packet.ip, packet.port) < 0)
    {
        printf("??");
        return ;
    }
    
    lobbySendFirstConnectReq();
}





void NetworkHandler::lobbyHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::FirstConnectResPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (dataSize > sizeof(packet))
    {
        printf("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.channelNo, pData, sizeof(packet.channelNo));
    pData += sizeof(packet.channelNo);
    
    memcpy(&packet.channelNameLen, pData, sizeof(packet.channelNameLen));
    pData += sizeof(packet.channelNameLen);
    
    memcpy(packet.channelName, pData, packet.channelNameLen);
    pData += packet.channelNameLen;
    
    if(pData - data != dataSize)
    {
        printf("dataSize = %d, - data = %d", dataSize, (int)(pData - data));
        return ;
    }
    
    printf("%d %d %s\n", packet.channelNo, packet.channelNameLen, packet.channelName);
    
    ///////////////////send create room req
    
    
    frontSendEnterLobbyOk();
    
    network->disconnectWithServer(SERVER_MODULE_FRONT_SERVER);
    
    

//    NetworkLayer* networkLyr = (NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER);
//    
//    Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOGIN_SCENE)->removeChildByTag(TAG_NETWORK_LAYER, false);
//    
//    auto scene = LobbyScene::createScene();
//    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->AddThisToScene(scene);
    
}

void NetworkHandler::lobbyHandleFirstConnectOk(ConnectInfo* connectInfo, const char* data, int dataSize)
{
// TODO. receiveUserList
    
    printf("ok");
    
    Scene* scene = LobbyScene::createScene();
    
    ((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->AddThisToScene(scene);
    Director::getInstance()->replaceScene(scene);
}

void NetworkHandler::lobbyHandleGetChannelListRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::GetChannelListResPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (dataSize > sizeof(packet))
    {
        log("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.channelCount, pData, sizeof(packet.channelCount));
    pData += sizeof(packet.channelCount);
    
    for(int i = 0; i <packet.channelCount; i++)
    {
        memcpy(&packet.channelInfoList[i].channelNo, pData, sizeof(packet.channelInfoList[i].channelNo));
        pData += sizeof(packet.channelInfoList[i].channelNo);
        
        memcpy(&packet.channelInfoList[i].channelNameLen, pData, sizeof(packet.channelInfoList[i].channelNameLen));
        pData += sizeof(packet.channelInfoList[i].channelNameLen);
        
        memcpy(&packet.channelInfoList[i].channelName, pData, packet.channelInfoList[i].channelNameLen);
        pData += packet.channelInfoList[i].channelNameLen;
    }
    
    if(pData - data != dataSize)
    {
        printf("dataSize = %d, - data = %d", dataSize, (int)(pData - data));
        return ;
    }
    
    //printf("%d %d %s\n", packet.channelNo, packet.channelNameLen, packet.channelName);
    
    int channelCount = packet.channelCount;
    ChannelInfo channelInfoList[channelCount];
    memset(channelInfoList, 0, sizeof(ChannelInfo) * channelCount);
    for(int i = 0; i < channelCount; i++)
    {
        channelInfoList[i].channelNo = packet.channelInfoList[i].channelNo;
        memcpy(channelInfoList[i].channelName, packet.channelInfoList[i].channelName, packet.channelInfoList[i].channelNameLen);
    }
    
  //  ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->SetChannelListLayer(channelCount, channelInfoList);
}


void NetworkHandler::lobbyHandleMoveChannelRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::MoveChannelResPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (dataSize > sizeof(packet))
    {
        log("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.channelInfo.channelNo, pData, sizeof(packet.channelInfo.channelNo));
    pData += sizeof(packet.channelInfo.channelNo);
    
    memcpy(&packet.channelInfo.channelNameLen, pData, sizeof(packet.channelInfo.channelNameLen));
    pData += sizeof(packet.channelInfo.channelNameLen);
    
    memcpy(&packet.channelInfo.channelName, pData, packet.channelInfo.channelNameLen);
    pData += packet.channelInfo.channelNameLen;

    if(pData - data != dataSize)
    {
        printf("dataSize = %d, - data = %d", dataSize, (int)(pData - data));
        return ;
    }
    
    //printf("%d %d %s\n", packet.channelNo, packet.channelNameLen, packet.channelName);
    
    
    ChannelInfo channelInfo;
    memset(&channelInfo, 0, sizeof(ChannelInfo));
    channelInfo.channelNo = packet.channelInfo.channelNo;
    memcpy(channelInfo.channelName, packet.channelInfo.channelName, packet.channelInfo.channelNameLen);
    
    
    GameClient::GetInstance().channelNo = channelInfo.channelNo;
    
 //   ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->MoveChannel(channelInfo.channelNo, channelInfo.channelName);
}


void NetworkHandler::lobbyHandleGetRoomListRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::GetRoomListResPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (dataSize > sizeof(packet))
    {
        log("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.roomCount, pData, sizeof(packet.roomCount));
    pData += sizeof(packet.roomCount);
    
    
    for(int i = 0; i <packet.roomCount; i++)
    {
        memcpy(&packet.roomInfoList[i].roomNo, pData, sizeof(packet.roomInfoList[i].roomNo));
        pData += sizeof(packet.roomInfoList[i].roomNo);
        
        memcpy(&packet.roomInfoList[i].roomNameLen, pData, sizeof(packet.roomInfoList[i].roomNameLen));
        pData += sizeof(packet.roomInfoList[i].roomNameLen);
        
        memcpy(&packet.roomInfoList[i].roomName, pData, packet.roomInfoList[i].roomNameLen);
        pData += packet.roomInfoList[i].roomNameLen;
    }
    
    if(pData - data != dataSize)
    {
        printf("dataSize = %d, - data = %d", dataSize, (int)(pData - data));
        return ;
    }
    
    //printf("%d %d %s\n", packet.channelNo, packet.channelNameLen, packet.channelName);
    
    int roomCount = packet.roomCount;
    RoomInfo roomInfoList[roomCount];
    memset(roomInfoList, 0, sizeof(RoomInfo) * roomCount);
    for(int i = 0; i < roomCount; i++)
    {
        roomInfoList[i].roomNo = packet.roomInfoList[i].roomNo;
        memcpy(roomInfoList[i].roomName, packet.roomInfoList[i].roomName, packet.roomInfoList[i].roomNameLen);
    }
    
   // ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->SetRoomListLayer(roomCount, roomInfoList);
}


void NetworkHandler::lobbyHandleCreateRoomRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
    printf("NetworkHandler::lobbyHandleCreateRoomRes");
    ClientLobbyPacket::CreateRoomResPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (dataSize > sizeof(packet))
    {
        log("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.roomDetailInfo.roomNo, pData, sizeof(packet.roomDetailInfo.roomNo));
    pData += sizeof(packet.roomDetailInfo.roomNo);
    
    memcpy(&packet.roomDetailInfo.roomNameLen, pData, sizeof(packet.roomDetailInfo.roomNameLen));
    pData += sizeof(packet.roomDetailInfo.roomNameLen);
    
    memcpy(&packet.roomDetailInfo.roomName, pData, packet.roomDetailInfo.roomNameLen);
    pData += packet.roomDetailInfo.roomNameLen;
    
    memcpy(&packet.roomDetailInfo.mapType, pData, sizeof(packet.roomDetailInfo.mapType));
    pData += sizeof(packet.roomDetailInfo.mapType);
    
    memcpy(&packet.roomDetailInfo.masterIndex, pData, sizeof(packet.roomDetailInfo.masterIndex));
    pData += sizeof(packet.roomDetailInfo.masterIndex);
    
    for(int i = 0; i < MAX_ROOM_SLOT_COUNT; i++)
    {
        memcpy(&packet.roomDetailInfo.roomSlotList[i].isOpen, pData, sizeof(packet.roomDetailInfo.roomSlotList[i].isOpen));
        pData += sizeof(packet.roomDetailInfo.roomSlotList[i].isOpen);
        
        if(packet.roomDetailInfo.roomSlotList[i].isOpen == 1)
        {
            
            memcpy(&packet.roomDetailInfo.roomSlotList[i].isExistUser, pData, sizeof(packet.roomDetailInfo.roomSlotList[i].isExistUser));
            pData += sizeof(packet.roomDetailInfo.roomSlotList[i].isExistUser);
            
            if(packet.roomDetailInfo.roomSlotList[i].isExistUser == 1)
            {
                memcpy(&packet.roomDetailInfo.roomSlotList[i].isReady, pData, sizeof(packet.roomDetailInfo.roomSlotList[i].isReady));
                pData += sizeof(packet.roomDetailInfo.roomSlotList[i].isReady);
                
                memcpy(&packet.roomDetailInfo.roomSlotList[i].tribe, pData, sizeof(packet.roomDetailInfo.roomSlotList[i].tribe));
                pData += sizeof(packet.roomDetailInfo.roomSlotList[i].tribe);
                
                memcpy(&packet.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen, pData, sizeof(packet.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen));
                pData += sizeof(packet.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen);
                
                memcpy(packet.roomDetailInfo.roomSlotList[i].nickNameInfo.nickName, pData, packet.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen);
                pData += packet.roomDetailInfo.roomSlotList[i].nickNameInfo.nickNameLen;
            }
        }
    }
    
    if(pData - data != dataSize)
    {
        printf("dataSize = %d, - data = %d", dataSize, (int)(pData - data));
        return ;
    }
    
    lobbySendReadyReq();
}


void NetworkHandler::lobbyHandleEnterRoomRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleEnterRoomNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleReadyRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleReadyNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleUnReadyRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleUnReadyNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleChangeTribeRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleChangeTribeNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleLeaveRoomRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleLeaveRoomNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleStartGameNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("lobby start game");
    
    ClientLobbyPacket::StartGameNotifyPacket packet;
    
    const char* pData = data;
    
    memcpy(packet.ip, pData, MAX_IP_ADDRESS_LEN);
    pData += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.port, pData, sizeof(packet.port));
    pData += sizeof(packet.port);
    
    if(network->connectWithServer(SERVER_MODULE_GAME_SERVER, packet.ip, packet.port) < 0)
    {
        printf("??");
        return ;
    }
    
    gameSendFirstConnectReq();
}

void NetworkHandler::lobbyHandleQuickPlayRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("lobbyHandleQuickPlayRes");
    
    ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->waitToStart();
}





/////////////////game recv

void NetworkHandler::gameHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::gameHandleFirstConnectRes");
    
    ClientLobbyPacket::StartGameOkPacket packet;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));
}


void NetworkHandler::gameHandleStartGameNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::gameHandleStartGameNotify");

    Scene* scene = GameWorld::createScene();
    
    ((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->AddThisToScene(scene);
    Director::getInstance()->replaceScene(scene);
}


void NetworkHandler::gameHandleClientNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::gameHandleClientNotify");
    
    NetMgr->FetchFromServer(dataSize,data);
}



















/////////////////////////////////////////////////////////////////



void NetworkHandler::frontSendFirstConnectReq()
{
    ClientFrontPacket::FirstConnectReqPacket packet;
    
    memcpy(&packet.sessionId, &GameClient::GetInstance().sessionId, sizeof(packet.sessionId));
    
    
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_FRONT_SERVER, sendBuffer, sizeof(packet));
    
}


void NetworkHandler::frontSendEnterLobbyReq()
{
    ClientFrontPacket::EnterLobbyReqPacket packet;
    
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_FRONT_SERVER, (char*)&packet, sizeof(packet));
}

void NetworkHandler::frontSendEnterLobbyOk()
{
    ClientFrontPacket::EnterLobbyOkPacket packet;
    
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_FRONT_SERVER, (char*)&packet, sizeof(packet));
}


void NetworkHandler::lobbySendFirstConnectReq()
{
    ClientLobbyPacket::FirstConnectReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.cmd = ClientLobbyPacket::FIRST_CONNECT_REQ;
    memcpy(&packet.sid, &GameClient::GetInstance().sessionId, sizeof(packet.sid));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));
    
}


void NetworkHandler::lobbySendGetChannelListReq()
{
    ClientLobbyPacket::GetChannelListReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.cmd = ClientLobbyPacket::GET_CHANNEL_LIST_REQ;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet.cmd, sizeof(packet.cmd));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet.cmd));
}


void NetworkHandler::lobbySendMoveChannelReq(int channelNo)
{
    ClientLobbyPacket::MoveChannelReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    
    packet.cmd = ClientLobbyPacket::MOVE_CHANNEL_REQ;
    packet.channelNo = channelNo;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet));
}

void NetworkHandler::lobbySendGetRoomListReq()
{
    ClientLobbyPacket::GetRoomListReqPacket packet;
    packet.cmd = ClientLobbyPacket::GET_ROOM_LIST_REQ;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet.cmd, sizeof(packet.cmd));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet.cmd));
    
}

void NetworkHandler::lobbySendCreateRoomReq(const char* roomName, int roomNameLen, int mapType)
{
    ClientLobbyPacket::CreateRoomReqPacket packet;
    packet.cmd = ClientLobbyPacket::CREATE_ROOM_REQ;
    packet.roomInfo.roomNo = 0;
    packet.roomInfo.roomNameLen = roomNameLen;
    memcpy(packet.roomInfo.roomName, roomName, roomNameLen);
    packet.mapType = mapType;
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);
    
    memcpy(pSendBuffer, &packet.roomInfo.roomNo, sizeof(packet.roomInfo.roomNo));
    pSendBuffer += sizeof(packet.roomInfo.roomNo);
    
    
    memcpy(pSendBuffer, &packet.roomInfo.roomNameLen, sizeof(packet.roomInfo.roomNameLen));
    pSendBuffer += sizeof(packet.roomInfo.roomNameLen);
    
    memcpy(pSendBuffer, &packet.roomInfo.roomName, packet.roomInfo.roomNameLen);
    pSendBuffer += packet.roomInfo.roomNameLen;
    
    memcpy(pSendBuffer, &packet.mapType, sizeof(packet.mapType));
    pSendBuffer += sizeof(packet.mapType);
    
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, (int)(pSendBuffer - sendBuffer));
}

void NetworkHandler::lobbySendEnterRoomReq(int roomNo)
{
    ClientLobbyPacket::EnterRoomReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    
    packet.cmd = ClientLobbyPacket::ENTER_ROOM_REQ;
    packet.roomNo = roomNo;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet));
    
}

void NetworkHandler::lobbySendReadyReq()
{
    ClientLobbyPacket::ReadyReqPacket packet;
    packet.cmd = ClientLobbyPacket::READY_REQ;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet.cmd, sizeof(packet.cmd));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet.cmd));
}

void NetworkHandler::lobbySendUnReadyReq()
{
    ClientLobbyPacket::UnreadyReqPacket packet;
    packet.cmd = ClientLobbyPacket::UNREADY_REQ;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet.cmd, sizeof(packet.cmd));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet.cmd));
    
}
void NetworkHandler::lobbySendChangeTribeReq(int tribe)
{
    ClientLobbyPacket::ChangeTribeReqPacket packet;
    memset(&packet, 0, sizeof(packet));
    
    packet.cmd = ClientLobbyPacket::CHANGE_TRIBE_REQ;
    packet.tribe = tribe;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet));
}


void NetworkHandler::lobbySendLeaveRoomReq()
{
    ClientLobbyPacket::LeaveRoomReqPacket packet;
    packet.cmd = ClientLobbyPacket::LEAVE_ROOM_REQ;
    
    memset(sendBuffer, 0, sizeof(sendBuffer));
    memcpy(sendBuffer, &packet.cmd, sizeof(packet.cmd));
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, sendBuffer, sizeof(packet.cmd));
    
}



void NetworkHandler::lobbySendQuickPlayReq(int tribe)
{
    ClientLobbyPacket::QuickPlayReqPacket packet;
    packet.tribe = tribe;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));
}


void NetworkHandler::gameSendFirstConnectReq()
{
    ClientGamePacket::FirstConnectReqPacket packet;
    
    memcpy(&packet.sid, &GameClient::GetInstance().sessionId, sizeof(packet.sid));
    
    
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_GAME_SERVER, sendBuffer, sizeof(packet));
}

void NetworkHandler::gameSendClientNotify(int bufferLen, const char* clientSendBuffer)
{
    ClientGamePacket::ClientNotifyPacket packet;
    
    char* pSendBuffer = sendBuffer;
    memcpy(pSendBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendBuffer += sizeof(packet.cmd);
    
    memcpy(pSendBuffer, clientSendBuffer, bufferLen);
    pSendBuffer += bufferLen;
    
    network->sendPacket(SERVER_MODULE_GAME_SERVER, sendBuffer, (int)(pSendBuffer - sendBuffer));
}




////////////////////////////////////////////










bool NetworkLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    handler = new NetworkHandler();
    if(handler->initialize() == false)
    {
        return false;
    }

    this->scheduleUpdate();

	return true;
}

void NetworkLayer::update(float dt)
{
    int recvBytes = 0;
    while (1)
    {
        recvBytes = handler->network->receiveData(SERVER_MODULE_FRONT_SERVER);
        if(recvBytes <= 0)
            break;
    }
    
    while (1)
    {
        recvBytes = handler->network->receiveData(SERVER_MODULE_LOBBY_SERVER);
        if(recvBytes <= 0)
            break;
    }
    
    while (1)
    {
        recvBytes = handler->network->receiveData(SERVER_MODULE_GAME_SERVER);
        if(recvBytes <= 0)
            break;
    }
}


void NetworkLayer::AddThisToScene(cocos2d::Scene* scene)
{
    NetworkLayer* nl = (NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER);
	Director::getInstance()->getRunningScene()->removeChildByTag(TAG_NETWORK_LAYER, false);
	scene->addChild(nl, 0, TAG_NETWORK_LAYER);
}





