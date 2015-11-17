#ifndef __CLIENT_LOBBY_PACKET_H__
#define __CLIENT_LOBBY_PACKET_H__

#include "BasicPacket.h"
//#include "NetworkDefines.h"

class ClientLobbyPacket
{
public:

	////////////Packet Command//////////////

	static const int32_t PACKET_VERSION = 7;

	enum EnumCommand // start number is at least 3.
	{
		FIRST_CONNECT_REQ = 10,
		FIRST_CONNECT_RES,
		FIRST_CONNECT_FAIL,
        FIRST_CONNECT_OK,
        FIRST_CONNECT_OUT,

		CHATTING_REQ,
		CHATTING_NOTIFY,
		CHATTING_FAIL,

		GET_CHANNEL_LIST_REQ,
		GET_CHANNEL_LIST_RES,
		GET_CHANNEL_LIST_FAIL,

		MOVE_CHANNEL_REQ,
		MOVE_CHANNEL_RES,
		MOVE_CHANNEL_FAIL,
        
        GET_USER_LIST_REQ,
        GET_USER_LIST_RES,
        GET_USER_LIST_FAIL,
        
        GET_USER_INFO_REQ,
        GET_USER_INFO_RES,
        GET_USER_INFO_FAIL,
        
        CHANNEL_INFO_NOTIFY,
        ENTER_USER_IN_CHANNEL_NOTIFY,
        
        LEAVE_USER_IN_CHANNEL_NOTIFY,
        
        REQUEST_GAME_REQ,
        REQUEST_GAME_RES,
        REQUEST_GAME_NOTIFY,
        REQUEST_GAME_FAIL,
        
        REQUEST_GAME_CANCEL_REQ,
        REQUEST_GAME_CANCEL_RES,
        REQUEST_GAME_CANCEL_FAIL,
        
        RESPONSE_GAME_YES_REQ,
        RESPONSE_GAME_YES_RES,
        RESPONSE_GAME_YES_FAIL,

        RESPONSE_GAME_NO_REQ,
        RESPONSE_GAME_NO_RES,
        RESPONSE_GAME_NO_NOTIFY,
        RESPONSE_GAME_NO_FAIL,
        
        GET_ROOM_LIST_REQ,
        GET_ROOM_LIST_RES,
        GET_ROOM_LIST_FAIL,
        
        CREATE_ROOM_REQ,
        CREATE_ROOM_RES,
        CREATE_ROOM_FAIL,
        
        ENTER_ROOM_REQ,
        ENTER_ROOM_RES,
        ENTER_ROOM_NOTIFY,
        ENTER_ROOM_FAIL,
        
        READY_REQ,
        READY_RES,
        READY_NOTIFY,
        READY_FAIL,
        
        UNREADY_REQ,
        UNREADY_RES,
        UNREADY_NOTIFY,
        UNREADY_FAIL,
        
        CHANGE_TRIBE_REQ,
        CHANGE_TRIBE_RES,
        CHANGE_TRIBE_NOTIFY,
        CHANGE_TRIBE_FAIL,
        
        LEAVE_ROOM_REQ,
        LEAVE_ROOM_RES,
        LEAVE_ROOM_NOTIFY,
        LEAVE_ROOM_FAIL,
        
        START_GAME_NOTIFY,
        START_GAME_OK,
        
        QUICK_PLAY_REQ,
        QUICK_PLAY_RES,
        QUICK_PLAY_FAIL,
        
        QUICK_PLAY_CANCEL_REQ,
        QUICK_PLAY_CANCEL_RES,
        QUICK_PLAY_CANCEL_FAIL,
        
        QUICK_PLAY_NOTIFY,
        QUICK_PLAY_OK,

		GET_FRIEND_LIST_REQ,
		GET_FRIEND_LIST_RES,
		GET_FRIEND_LIST_FAIL,

		ADD_REQUEST_FRIEND_RES,
		ADD_REQUEST_FRIEND_FAIL,
		ADD_REQUEST_FRIEND_NOTIFY,

		ADD_RESPONSE_FRIEND_REQ,
		ADD_RESPONSE_FRIEND_RES,
		ADD_RESPONSE_FRIEND_FAIL,

		REMOVE_FRIEND_RES,
		REMOVE_FRIEND_FAIL,

		LOGOUT_USER_REQ,
		LOGOUT_USER_RES,
		LOGOUT_USER_FAIL,
	};

	enum EnumFailReason
	{
		PACKET_VERSION_INCORRECT = 1,
		SERVER_ERROR,
		ALREADY_REQUEST,
		ALREADY_FRIEND,
		NOT_FRIEND,
	};

	///////////////////////////////////

#pragma pack(push, 1)

	///////////////////////////////////


    struct FirstConnectReqPacket : public Packet
	{
        FirstConnectReqPacket() { cmd = FIRST_CONNECT_REQ; }
        
		SessionId_t sid;
	};

    struct FirstConnectResPacket : public Packet
	{
        FirstConnectResPacket() { cmd = FIRST_CONNECT_RES; }

        char chattingIp[MAX_IP_ADDRESS_LEN];
        int16_t chattingPort;
        
        UserInfo userInfo;
        
        int8_t friendCount;
        NickNameInfoWithOnline nickNameInfoWithOnlineList[MAX_FRIEND_COUNT];
        
        ClanInfo clanInfo;
	};
    
    struct FirstConnectOkPacket : public Packet
    {
        FirstConnectOkPacket() { cmd = FIRST_CONNECT_OK; }
        
    };
    
    struct FirstConnectOutPacket : public Packet
    {
        FirstConnectOutPacket() { cmd = FIRST_CONNECT_OUT; }
        
    };

    struct GetChannelListReqPacket : public Packet
    {
        GetChannelListReqPacket() { cmd = GET_CHANNEL_LIST_REQ; }
        
    };
    
    struct GetChannelListResPacket : public Packet
    {
        GetChannelListResPacket() { cmd = GET_CHANNEL_LIST_RES; }
        
        int8_t channelCount;
        ChannelInfo channelInfoList[MAX_CHANNEL_COUNT];
    };
    
    struct MoveChannelReqPacket : public Packet
    {
        MoveChannelReqPacket() { cmd = MOVE_CHANNEL_REQ; }
        
        int16_t channelNo;
    };
    
    struct MoveChannelResPacket : public Packet
    {
        MoveChannelResPacket() { cmd = MOVE_CHANNEL_RES; }
        
        ChannelInfo channelInfo;
    };

    struct GetUserListReqPacket : public Packet
    {
        GetUserListReqPacket() { cmd = GET_USER_LIST_REQ; }
        
    };
    
    struct GetUserListResPacket : public Packet
    {
        GetUserListResPacket() { cmd = GET_USER_LIST_RES; }
        
        uint8_t userCount;
        NickNameInfo nickNameInfoList[MAX_USER_COUNT_IN_CHANNEL];
    };

    struct GetUserInfoReqPacket : public Packet
    {
        GetUserInfoReqPacket() { cmd = GET_USER_INFO_REQ; }
        
        NickNameInfo nickNameInfo;
    };

    struct GetUserInfoResPacket : public Packet
    {
        GetUserInfoResPacket() { cmd = GET_USER_INFO_RES; }
        
        UserInfo userInfo;
    };
    
    struct ChannelInfoNotifyPacket : public Packet
    {
        ChannelInfoNotifyPacket() { cmd = CHANNEL_INFO_NOTIFY; }
        
        int16_t channelNo;
        int8_t channelNameLen;
        char channelName[MAX_CHANNEL_NAME_LEN];
        
        uint8_t userCount;
        NickNameInfo nickNameInfoList[MAX_USER_COUNT_IN_CHANNEL];
    };
    
    
    struct EnterUserInChannelNotifyPacket : public Packet
    {
        EnterUserInChannelNotifyPacket() { cmd = ENTER_USER_IN_CHANNEL_NOTIFY; }
    
        NickNameInfo nickNameInfo;
    };

    struct LeaveUserInChannelNotifyPacket : public Packet
    {
        LeaveUserInChannelNotifyPacket() { cmd = LEAVE_USER_IN_CHANNEL_NOTIFY; }
        
        NickNameInfo nickNameInfo;
    };
    
    
    struct RequestGameReqPacket : public Packet
    {
        RequestGameReqPacket() { cmd = REQUEST_GAME_REQ; }
        
        NickNameInfo nickNameInfo;
    };
    
    struct RequestGameResPacket : public Packet
    {
        RequestGameResPacket() { cmd = REQUEST_GAME_RES; }
        
        
    };
    
    struct RequestGameNotifyPacket : public Packet
    {
        RequestGameNotifyPacket() { cmd = REQUEST_GAME_NOTIFY; }
        
        NickNameInfo nickNameInfo;
    };
    
    
    struct RequestGameCancelReqPacket : public Packet
    {
        RequestGameCancelReqPacket() { cmd = REQUEST_GAME_CANCEL_REQ; }
        
    };
    
    struct RequestGameCancelResPacket : public Packet
    {
        RequestGameCancelResPacket() { cmd = REQUEST_GAME_CANCEL_RES; }
        
    };
    
    struct ResponseGameYesReqPacket : public Packet
    {
        ResponseGameYesReqPacket() { cmd = RESPONSE_GAME_YES_REQ; }
        
        NickNameInfo nickNameInfo;
    };
   
    
    struct ResponseGameYesResPacket : public Packet //
    {
        ResponseGameYesResPacket() { cmd = RESPONSE_GAME_YES_RES; }
        
    };
    
    
    struct ResponseGameNoReqPacket : public Packet
    {
        ResponseGameNoReqPacket() { cmd = RESPONSE_GAME_NO_REQ; }
        
        NickNameInfo nickNameInfo;
    };
    
    
    struct ResponseGameNoResPacket : public Packet //
    {
        ResponseGameNoResPacket() { cmd = RESPONSE_GAME_NO_RES; }
        
    };
    
    struct ResponseGameNoNotifyPacket : public Packet
    {
        ResponseGameNoNotifyPacket() { cmd = RESPONSE_GAME_NO_NOTIFY; }
        
    };
    

    
    
    
    struct GetRoomListReqPacket : public Packet
    {
        GetRoomListReqPacket() { cmd = GET_ROOM_LIST_REQ; }
    };
    
    struct GetRoomListResPacket : public Packet
    {
        GetRoomListResPacket() { cmd = GET_ROOM_LIST_RES; }
        
        int8_t roomCount;
        RoomInfo roomInfoList[MAX_ROOM_COUNT];
    };
    
    struct CreateRoomReqPacket : public Packet
    {
        CreateRoomReqPacket() { cmd = CREATE_ROOM_REQ; }
        RoomInfo roomInfo;
        int8_t mapType;
    };
    
    struct CreateRoomResPacket : public Packet
    {
        CreateRoomResPacket() { cmd = CREATE_ROOM_RES; }
        
        RoomDetailInfo roomDetailInfo;
    };
    
    struct EnterRoomReqPacket : public Packet
    {
        EnterRoomReqPacket() { cmd = ENTER_ROOM_REQ; }
        
        int16_t roomNo;
    };
    
    struct EnterRoomResPacket : public Packet
    {
        EnterRoomResPacket() { cmd = ENTER_ROOM_RES; }
        
        int8_t myIndex;
        RoomDetailInfo roomDetailInfo;
    };
    
    struct EnterRoomNotifyPacket : public Packet
    {
        EnterRoomNotifyPacket() { cmd = ENTER_ROOM_NOTIFY; }
        
        int8_t enterIndex;
        RoomSlot roomSlot;
    };
    
    struct ReadyReqPacket : public Packet
    {
        ReadyReqPacket() { cmd = READY_REQ; }
        
    };
    
    struct ReadyResPacket : public Packet
    {
        ReadyResPacket() { cmd = READY_RES; }
    };
    
    struct ReadyNotifyPacket : public Packet
    {
        ReadyNotifyPacket() { cmd = READY_NOTIFY; }
        
        int8_t readyIndex;
    };
    
    struct UnreadyReqPacket : public Packet
    {
        UnreadyReqPacket() { cmd = UNREADY_REQ; }
    };
    
    struct UnreadyResPacket : public Packet
    {
        UnreadyResPacket() { cmd = UNREADY_RES; }
    };
    
    struct UnreadyNotifyPacket : public Packet
    {
        UnreadyNotifyPacket() { cmd = UNREADY_NOTIFY; }
        
        int8_t unreadyIndex;
    };
    
    struct ChangeTribeReqPacket : public Packet
    {
        ChangeTribeReqPacket() { cmd = CHANGE_TRIBE_REQ; }
        int8_t tribe;
    };
    
    struct ChangeTribeResPacket : public Packet
    {
        ChangeTribeResPacket() { cmd = CHANGE_TRIBE_RES; }
        
        int8_t tribe;
    };
    
    struct ChangeTribeNotifyPacket : public Packet
    {
        ChangeTribeNotifyPacket() { cmd = CHANGE_TRIBE_NOTIFY; }
        
        int8_t changeTribeIndex;
        int8_t tribe;
    };
    
    struct LeaveRoomReqPacket : public Packet
    {
        LeaveRoomReqPacket() { cmd = LEAVE_ROOM_REQ; }
    };
    
    struct LeaveRoomResPacket : public Packet //if move lobby server??
    {
        LeaveRoomResPacket() { cmd = LEAVE_ROOM_RES; }
        
        ChannelInfo channelInfo;

    };
    
    struct LeaveRoomNotifyPacket : public Packet
    {
        LeaveRoomNotifyPacket() { cmd = LEAVE_ROOM_NOTIFY; }
        
        int8_t leaveIndex;
        RoomSlot roomSlot;
    };
    
    struct StartGameNotifyPacket : public Packet
    {
        StartGameNotifyPacket() { cmd = START_GAME_NOTIFY; }
        
        int8_t userCount;
        GameUserInfo gameUserInfo[MAX_ROOM_SLOT_COUNT];
        
        char ip[MAX_IP_ADDRESS_LEN];
        uint16_t port;
    };
    
    struct StartGameOkPacket : public Packet
    {
        StartGameOkPacket() { cmd = START_GAME_OK; }
        
    };
    
    
    struct QuickPlayReqPacket : public Packet
    {
        QuickPlayReqPacket() { cmd = QUICK_PLAY_REQ; }
        
        int8_t tribe;
    };
    
    struct QuickPlayResPacket : public Packet
    {
        QuickPlayResPacket() { cmd = QUICK_PLAY_RES; }
        
    };
    
    
    struct QuickPlayCancelReqPacket : public Packet
    {
        QuickPlayCancelReqPacket() { cmd = QUICK_PLAY_CANCEL_REQ; }
        
    };
    
    struct QuickPlayCancelResPacket : public Packet
    {
        QuickPlayCancelResPacket() { cmd = QUICK_PLAY_CANCEL_RES; }
        
    };
    
    struct QuickPlayNotifyPacket : public Packet
    {
        QuickPlayNotifyPacket() { cmd = QUICK_PLAY_NOTIFY; }
        
        int8_t userCount;
        GameUserInfo gameUserInfo[MAX_ROOM_SLOT_COUNT];
        
        char ip[MAX_IP_ADDRESS_LEN];
        uint16_t port;
    };
    
    struct QuickPlayOkPacket : public Packet
    {
        QuickPlayOkPacket() { cmd = QUICK_PLAY_OK; }
        
    };
    
    
    
	struct FailPacket
	{
		int32_t failReason;
	};
};

#pragma pack(pop)
#endif //__CLIENT_LOBBY_PACKET_H__
