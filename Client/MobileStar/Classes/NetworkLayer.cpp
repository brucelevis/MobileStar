#include "NetworkLayer.h"
#include "GameDefines.h"
#include "LoginScene.h"
#include "GameClient.h"
#include "LobbyScene.h"
#include "LoginScene.h"
#include "ResultScene.h"
#include "ClientFrontPacket.h"
#include "ClientLobbyPacket.h"
#include "ClientGamePacket.h"
#include "ClientChattingPacket.h"
#include "UserListLayer.h"
#include "GameWorld.h"

#include "LobbyChannelLayer.h"

#include "BasicPacket.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MyObject-C-Interface.h"
#endif



USING_NS_CC;

bool NetworkHandler::initialize()
{
    network = new Network();
    if(network->initialize(this) == false)
    {
        return false;
    }
    
    //127.0.0.1
    
    if (network->connectWithServer(SERVER_MODULE_FRONT_SERVER, IP_ADDRESS, PORT) == false)
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
                
            case ClientFrontPacket::LOGIN_RES:
                frontHandleLoginRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientFrontPacket::SIGN_UP_RES:
                frontHandleSignUpRes(connectInfo, pData, pDataSize);
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
                
            case ClientLobbyPacket::CHANNEL_INFO_NOTIFY:
                lobbyHandleChannelInfoNotify(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::FIRST_CONNECT_OK:
                lobbyHandleFirstConnectOk(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::FIRST_CONNECT_OUT:
                lobbyHandleFirstConnectOut(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::GET_CHANNEL_LIST_RES:
                lobbyHandleGetChannelListRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientLobbyPacket::MOVE_CHANNEL_RES:
                lobbyHandleMoveChannelRes(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::GET_USER_INFO_RES:
                lobbyHandleGetUserInfoRes(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::ENTER_USER_IN_CHANNEL_NOTIFY:
                lobbyHandleEnterUserInChannelNotify(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::LEAVE_USER_IN_CHANNEL_NOTIFY:
                lobbyHandleLeaveUserInChannelNotify(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::REQUEST_GAME_RES:
                lobbyHandleRequestGameRes(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::REQUEST_GAME_NOTIFY:
                lobbyHandleRequestGameNotify(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::REQUEST_GAME_CANCEL_RES:
                lobbyHandleRequestGameCancelRes(connectInfo, pData, pDataSize);
                break;
                
                
            case ClientLobbyPacket::RESPONSE_GAME_NO_NOTIFY:
                lobbyHandleResponseGameNoNotify(connectInfo, pData, pDataSize);
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
                
            case ClientGamePacket::FINISH_GAME_RES:
                gameHandleFinishGameRes(connectInfo, pData, pDataSize);
                break;
                
                
            default:
                printf("type invalid - %d", cmd);
                break;
                
        }
    }
    else if(connectInfo->serverModule == SERVER_MODULE_CHATTING_SERVER)
    {
        switch (cmd)
        {
            case ClientChattingPacket::FIRST_CONNECT_RES:
                chattingHandleFirstConnectRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientChattingPacket::MOVE_LOCATION_RES:
                chattingHandleMoveLocationRes(connectInfo, pData, pDataSize);
                break;
                
            case ClientChattingPacket::SEND_CHATTING_NOTIFY:
                chattingHandleSendChattingNotify(connectInfo, pData, pDataSize);
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
    
    ClientFrontPacket::FirstConnectResPacket packet;
    memcpy(&packet.sessionId, data, sizeof(packet.sessionId));
    
    memcpy(&GameClient::GetInstance().sessionId, &packet.sessionId, sizeof(packet.sessionId));
    
    ((LoginScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOGIN_SCENE))->completeFirstConnect();
}

void NetworkHandler::frontHandleLoginRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::frontHandleLoginRes");
    
    ///////////////////TODO. draw waitting bar for user
    
    ClientFrontPacket::LoginResPacket packet;
    memcpy(&packet.userInfo, data, sizeof(packet.userInfo));
    
    printf("%lld", packet.userInfo.userNo);
    
    GameClient::GetInstance().userInfo->userNo = packet.userInfo.userNo;
    
    ///////////////////send enter lobby req
    
    frontSendEnterLobbyReq();
}


void NetworkHandler::frontHandleSignUpRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::frontHandleLoginRes");
    
    ///////////////////TODO. draw waitting bar for user
    
    ClientFrontPacket::SignUpResPacket packet;
    
    ((LoginScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOGIN_SCENE))->completeSignUp();
}


void NetworkHandler::frontHandleEnterLobbyRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::frontHandleEnterLobbyRes");
    
    ClientFrontPacket::EnterLobbyResPacket packet;
    
    ((LoginScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOGIN_SCENE))->completeSignUp();
}





void NetworkHandler::lobbyHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::FirstConnectResPacket packet;
    
    memcpy(packet.chattingIp, data, MAX_IP_ADDRESS_LEN);
    data += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.chattingPort, data, sizeof(packet.chattingPort));
    
    if(network->connectWithServer(SERVER_MODULE_CHATTING_SERVER, IP_ADDRESS, packet.chattingPort) < 0)
    {
        printf("??");
        return ;
    }
    
    chattingSendFirstConnectReq();
    
    
    int currentScene = GameClient::GetInstance().currentScene;
    
    if(currentScene == LOGIN_SCENE_NOW)
    {
        frontSendEnterLobbyOk();
    
        network->disconnectWithServer(SERVER_MODULE_FRONT_SERVER);
    }
    else if(currentScene == RESULT_SCENE_NOW)
    {
        gameSendMoveLobbyOk();
    
        network->disconnectWithServer(SERVER_MODULE_GAME_SERVER);
    }
    
    GameClient::GetInstance().currentScene = NO_SCENE_NOW;

    Scene* scene = LobbyScene::createScene();
    
    ((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->AddThisToScene(scene);
    Director::getInstance()->replaceScene(scene);
}


void NetworkHandler::lobbyHandleChannelInfoNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::ChannelInfoNotifyPacket packet;
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
    
    memcpy(&packet.userCount, pData, sizeof(packet.userCount));
    pData += sizeof(packet.userCount);
    
    for(int i = 0; i < packet.userCount; i++)
    {
        memcpy(&packet.userViewInfoList[i].userNo, pData, sizeof(packet.userViewInfoList[i].userNo));
        pData += sizeof(packet.userViewInfoList[i].userNo);
        
        memcpy(&packet.userViewInfoList[i].nickNameInfo.nickNameLen, pData, sizeof(packet.userViewInfoList[i].nickNameInfo.nickNameLen));
        pData += sizeof(packet.userViewInfoList[i].nickNameInfo.nickNameLen);
        
        memcpy(packet.userViewInfoList[i].nickNameInfo.nickName, pData, packet.userViewInfoList[i].nickNameInfo.nickNameLen);
        pData += packet.userViewInfoList[i].nickNameInfo.nickNameLen;
    }
    
    
    if(pData - data != dataSize)
    {
        printf("dataSize = %d, - data = %d", dataSize, (int)(pData - data));
        return ;
    }
    
    printf("%d %d %s\n", packet.channelNo, packet.channelNameLen, packet.channelName);
    
    for(int i = 0; i < packet.userCount; i++)
    {
        ((LobbyScene*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE)))->addUserInfo(packet.userViewInfoList[i].userNo, packet.userViewInfoList[i].nickNameInfo.nickNameLen, packet.userViewInfoList[i].nickNameInfo.nickName);
    }
    
    if(GameClient::GetInstance().isConnectedWithChattingServer)
    {
        chattingSendMoveLocationReq(packet.channelNo);
    }
    
}

void NetworkHandler::lobbyHandleFirstConnectOk(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    GameClient::GetInstance().isConnectedWithLobbyServer = true;
    if(GameClient::GetInstance().isConnectedWithChattingServer)
        lobbySendMoveChannelReq(INVALID_CHANNEL_NO);
}

void NetworkHandler::lobbyHandleFirstConnectOut(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    exit(0);
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



void NetworkHandler::lobbyHandleGetUserInfoRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::lobbyHandleEnterUserInChannelNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::EnterUserInChannelNotifyPacket packet;
    if (dataSize > sizeof(packet))
    {
        log("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.userViewInfo.userNo, pData, sizeof(packet.userViewInfo.userNo));
    pData += sizeof(packet.userViewInfo.userNo);
    
    memcpy(&packet.userViewInfo.nickNameInfo.nickNameLen, pData, sizeof(packet.userViewInfo.nickNameInfo.nickNameLen));
    pData += sizeof(packet.userViewInfo.nickNameInfo.nickNameLen);
    
    memcpy(packet.userViewInfo.nickNameInfo.nickName, pData, packet.userViewInfo.nickNameInfo.nickNameLen);
    pData += packet.userViewInfo.nickNameInfo.nickNameLen;
    
    
    ((LobbyScene*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE)))->addUserInfo(packet.userViewInfo.userNo, packet.userViewInfo.nickNameInfo.nickNameLen, packet.userViewInfo.nickNameInfo.nickName);
}


void NetworkHandler::lobbyHandleLeaveUserInChannelNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientLobbyPacket::LeaveUserInChannelNotifyPacket packet;
    if (dataSize > sizeof(packet))
    {
        CCLOG("small");
        return;
    }
    
    const char* pData = data;
    
    memcpy(&packet.userNo, pData, sizeof(packet.userNo));
    pData += sizeof(packet.userNo);
    
    
    ((LobbyScene*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE)))->removeUserInfo(packet.userNo);
}


void NetworkHandler::lobbyHandleRequestGameRes(ConnectInfo* connectInfo, const char* pData, int pDataSize)
{
    
}


void NetworkHandler::lobbyHandleRequestGameNotify(ConnectInfo* connectInfo, const char* pData, int pDataSize)
{
    
}


void NetworkHandler::lobbyHandleRequestGameCancelRes(ConnectInfo* connectInfo, const char* pData, int pDataSize)
{
    
}


void NetworkHandler::lobbyHandleResponseGameNoNotify(ConnectInfo* connectInfo, const char* pData, int pDataSize)
{
    
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
    
    memcpy(&packet.userCount, pData, sizeof(packet.userCount));
    pData += sizeof(packet.userCount);
    
    GameClient::GetInstance().userCount = packet.userCount;
    
    
    for(int i = 0; i < packet.userCount; i++)
    {
        memcpy(&packet.gameUserInfo[i].userNo, pData, sizeof(packet.gameUserInfo[i].userNo));
        pData += sizeof(packet.gameUserInfo[i].userNo);
    
        memcpy(&packet.gameUserInfo[i].tribe, pData, sizeof(packet.gameUserInfo[i].tribe));
        pData += sizeof(packet.gameUserInfo[i].tribe);
        printf("%lld\n", packet.gameUserInfo[i].userNo);
        GameClient::GetInstance().gameUserInfo[i].userNo = packet.gameUserInfo[i].userNo;
        GameClient::GetInstance().gameUserInfo[i].tribe = packet.gameUserInfo[i].tribe;
        
        if(GameClient::GetInstance().GetUserInfo()->userNo == packet.gameUserInfo[i].userNo)
        {
            printf("ook");
            GameClient::GetInstance().myGameIndex = i;
        }
    }
    
    memcpy(packet.ip, pData, MAX_IP_ADDRESS_LEN);
    pData += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.port, pData, sizeof(packet.port));
    pData += sizeof(packet.port);
    
    
    
    
    //    if(network->connectWithServer(SERVER_MODULE_GAME_SERVER, packet.ip, packet.port) < 0)
    if(network->connectWithServer(SERVER_MODULE_GAME_SERVER, IP_ADDRESS, packet.port) < 0)
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
    
    auto glview = Director::getInstance()->getOpenGLView();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "CallJavaFunction", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }

    glview->setFrameSize(glview->getFrameSize().height, glview->getFrameSize().width);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    MyObjectDoSomethingWith (NULL, NULL);
#endif
    
    glview->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, ResolutionPolicy::SHOW_ALL);

    
    GameClient::GetInstance().currentScene = NO_SCENE_NOW;
    
    Scene* scene = GameWorld::createScene();
    
    ((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->AddThisToScene(scene);
    Director::getInstance()->replaceScene(scene);
}


void NetworkHandler::gameHandleClientNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
//    printf("NetworkHandler::gameHandleClientNotify");
    
    NetMgr->FetchFromServer(dataSize,data);
}


void NetworkHandler::gameHandleFinishGameRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    printf("NetworkHandler::gameHandleFinishGameRes");

    ClientGamePacket::FinishGameResPacket packet;
    
    const char* pData = data;
    
    memcpy(packet.ip, pData, MAX_IP_ADDRESS_LEN);
    pData += MAX_IP_ADDRESS_LEN;
    
    memcpy(&packet.port, pData, sizeof(packet.port));
    pData += sizeof(packet.port);
    
    printf("%s %d", packet.ip, packet.port);
    
    if(network->connectWithServer(SERVER_MODULE_LOBBY_SERVER, IP_ADDRESS, packet.port) < 0)
    {
        printf("??");
        return ;
    }
    
    
    GameClient::GetInstance().currentScene = NO_SCENE_NOW;
    
    Scene* scene = ResultScene::createScene();
    
    ((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->AddThisToScene(scene);
    Director::getInstance()->replaceScene(scene);
    
    GameClient::GetInstance().currentScene = RESULT_SCENE_NOW;
}



/////////////////////////////////////////////////////////////

void NetworkHandler::chattingHandleFirstConnectRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    GameClient::GetInstance().isConnectedWithChattingServer = true;
    if(GameClient::GetInstance().isConnectedWithLobbyServer)
        lobbySendMoveChannelReq(INVALID_CHANNEL_NO);
}


void NetworkHandler::chattingHandleMoveLocationRes(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    
}


void NetworkHandler::chattingHandleSendChattingNotify(ConnectInfo* connectInfo, const char* data, int dataSize)
{
    ClientChattingPacket::SendChattingNotifyPacket packet;
    memcpy(&packet.nickNameLen, data, sizeof(packet.nickNameLen));
    data += sizeof(packet.nickNameLen);
    
    memcpy(&packet.nickName, data, packet.nickNameLen);
    data += packet.nickNameLen;

    memcpy(&packet.chattingLen, data, sizeof(packet.chattingLen));
    data += sizeof(packet.chattingLen);
    
    memcpy(&packet.chatting, data, packet.chattingLen);
    data += packet.chattingLen;
    
    CCLOG("%s %d %s %d", packet.nickName, packet.nickNameLen, packet.chatting, packet.chattingLen);
    
    if(GameClient::GetInstance().currentScene == LOBBY_SCENE_NOW)
    {
        ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->lobbyChannelLayer->notifyChatting(packet.nickName, packet.nickNameLen, packet.chatting, packet.chattingLen);
    }
}












/////////////////////////////////////////////////////////////////



void NetworkHandler::frontSendFirstConnectReq()
{
    ClientFrontPacket::FirstConnectReqPacket packet;
    
    packet.packetVersion = ClientFrontPacket::PACKET_VERSION;
    
    
    memcpy(sendBuffer, &packet, sizeof(packet));
    
    network->sendPacket(SERVER_MODULE_FRONT_SERVER, sendBuffer, sizeof(packet));
    
}

void NetworkHandler::frontSendLoginReq(const char* nickName, int8_t nickNameLen, const char* password, int8_t passwordLen)
{
    ClientFrontPacket::LoginReqPacket packet;
    
    packet.nickNameLen = nickNameLen;
    memcpy(packet.nickName, nickName, nickNameLen);
    packet.passwordLen = passwordLen;
    memcpy(packet.password, password, passwordLen);
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.nickNameLen, sizeof(packet.nickNameLen));
    pSendBuffer += sizeof(packet.nickNameLen);
    
    memcpy(pSendBuffer, packet.nickName, packet.nickNameLen);
    pSendBuffer += packet.nickNameLen;
    
    memcpy(pSendBuffer, &packet.passwordLen, sizeof(packet.passwordLen));
    pSendBuffer += sizeof(packet.passwordLen);
    
    memcpy(pSendBuffer, packet.password, packet.passwordLen);
    pSendBuffer += packet.passwordLen;
    
    network->sendPacket(SERVER_MODULE_FRONT_SERVER, sendBuffer, (int)(pSendBuffer - sendBuffer));
}

void NetworkHandler::frontSendSignUpReq(const char* nickName, int8_t nickNameLen, const char* password, int8_t passwordLen, const char* email, int8_t emailLen)
{
    ClientFrontPacket::SignUpReqPacket packet;
    
    packet.nickNameLen = nickNameLen;
    memcpy(packet.nickName, nickName, nickNameLen);
    packet.passwordLen = passwordLen;
    memcpy(packet.password, password, passwordLen);
    packet.emailLen = emailLen;
    memcpy(packet.email, email, emailLen);
    
    
    char* pSendBuffer = sendBuffer;
    
    memcpy(pSendBuffer, &packet.nickNameLen, sizeof(packet.nickNameLen));
    pSendBuffer += sizeof(packet.nickNameLen);
    
    memcpy(pSendBuffer, packet.nickName, packet.nickNameLen);
    pSendBuffer += packet.nickNameLen;
    
    memcpy(pSendBuffer, &packet.passwordLen, sizeof(packet.passwordLen));
    pSendBuffer += sizeof(packet.passwordLen);
    
    memcpy(pSendBuffer, packet.password, packet.passwordLen);
    pSendBuffer += packet.passwordLen;
    
    memcpy(pSendBuffer, &packet.emailLen, sizeof(packet.emailLen));
    pSendBuffer += sizeof(packet.emailLen);
    
    memcpy(pSendBuffer, packet.email, packet.emailLen);
    pSendBuffer += packet.emailLen;
    
    
    network->sendPacket(SERVER_MODULE_FRONT_SERVER, sendBuffer, (int)(pSendBuffer - sendBuffer));
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


void NetworkHandler::lobbySendGetUserInfoReq(int64_t userNo)
{
    ClientLobbyPacket::GetUserInfoReqPacket packet;
    packet.userNo = userNo;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));
}


void NetworkHandler::lobbySendRequestGameReq(int64_t userNo)
{
    ClientLobbyPacket::RequestGameReqPacket packet;
    packet.userNo = userNo;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));
    
}


void NetworkHandler::lobbySendRequestGameCancelReq()
{
    ClientLobbyPacket::RequestGameCancelReqPacket packet;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));

}


void NetworkHandler::lobbySendResponseGameYesReq(int64_t userNo)
{
    ClientLobbyPacket::ResponseGameYesReqPacket packet;
    packet.userNo = userNo;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));

}


void NetworkHandler::lobbySendResponseGameNoReq(int64_t userNo)
{
    ClientLobbyPacket::ResponseGameNoReqPacket packet;
    packet.userNo = userNo;
    
    network->sendPacket(SERVER_MODULE_LOBBY_SERVER, (char*)&packet, sizeof(packet));
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

void NetworkHandler::gameSendFinishGameReq()
{
    ClientGamePacket::FinishGameReqPacket packet;
    
    network->sendPacket(SERVER_MODULE_GAME_SERVER, (char*)&packet, sizeof(packet));
}


void NetworkHandler::gameSendMoveLobbyOk()
{
    ClientGamePacket::MoveLobbyOkPacket packet;
    
    network->sendPacket(SERVER_MODULE_GAME_SERVER, (char*)&packet, sizeof(packet));
}



void NetworkHandler::chattingSendFirstConnectReq()
{
    ClientChattingPacket::FirstConnectReqPacket packet;
    
    memcpy(&packet.sessionId, &GameClient::GetInstance().sessionId, sizeof(packet.sessionId));
    
    network->sendPacket(SERVER_MODULE_CHATTING_SERVER, (char*)&packet, sizeof(packet));
    
}


void NetworkHandler::chattingSendMoveLocationReq(int32_t loNo)
{
    ClientChattingPacket::MoveLocationReqPacket packet;
    
    packet.loNo = loNo;
    
    network->sendPacket(SERVER_MODULE_CHATTING_SERVER, (char*)&packet, sizeof(packet));
}

void NetworkHandler::chattingSendSendChattingReq(const char* chatting, uint8_t chattingLen)
{
    ClientChattingPacket::SendChattingReqPacket packet;
    
    packet.chattingLen = chattingLen;
    memcpy(packet.chatting, chatting, chattingLen);
    
    char sendChattingBuffer[300];
    char* pSendChattingBuffer = sendChattingBuffer;
    
    memcpy(pSendChattingBuffer, &packet.cmd, sizeof(packet.cmd));
    pSendChattingBuffer += sizeof(packet.cmd);
    
    memcpy(pSendChattingBuffer, &packet.chattingLen, sizeof(packet.chattingLen));
    pSendChattingBuffer += sizeof(packet.chattingLen);
    
    memcpy(pSendChattingBuffer, &packet.chatting, packet.chattingLen);
    pSendChattingBuffer += packet.chattingLen;
    
    network->sendPacket(SERVER_MODULE_CHATTING_SERVER, sendChattingBuffer, (int)(pSendChattingBuffer - sendChattingBuffer));
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
    
    while (1)
    {
        recvBytes = handler->network->receiveData(SERVER_MODULE_CHATTING_SERVER);
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

