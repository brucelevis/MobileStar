#ifndef __BASIC_PACKET_H__
#define __BASIC_PACKET_H__

#include <stdint.h>

typedef uint32_t command_t;

////////////Packet Command//////////////

static const int32_t PACKET_VERSION = 7;

static const int MAX_IP_ADDRESS_LEN = 15;
static const int MAX_EMAIL_LEN = 16; // check
static const int MAX_PASSWORD_LEN = 16; // check
static const int MAX_NICK_NAME_LEN = 16;
static const int MAX_ID_COUNT = 16; // check
static const int MAX_PASSWORD_COUNT = 16; // check
static const int MAX_CHANNEL_NAME_LEN = 16;
static const int MAX_ROOM_NAME_LEN = 16;
static const int MAX_CHATTING_LEN = 100;
static const int MAX_NICK_NAME_COUNT_IN_CHANNEL = 50;
static const int MAX_CHANNEL_COUNT = 10;
static const int MAX_ROOM_COUNT = 30;
static const int MAX_MEMO_LEN = 100;
static const int MAX_FRIEND_COUNT = 30;
static const int MAX_USER_COUNT_IN_CHANNEL = 200;
static const int MAX_REQUEST_INFO_COUNT = 10;

static const int MAX_ROOM_SLOT_COUNT = 2;

static const int INVALID_USER_NO = -1;
static const int INVALID_ROOM_NO = -1;
static const int INVALID_CHANNEL_NO = -1;

enum RequestType
{
    REQUEST_TYPE_ADD_REQUEST_FRIEND = 1,
    REQUEST_TYPE_ADD_RESPONSE_FRIEND,
};

enum TRIBE_TYPE
{
    TRIBE_TYPE_CHINA,
    TRIBE_TYPE_EUROPE,
    TRIBE_TYPE_USA,
};

///////////////////////////////////

#pragma pack(push, 1)

struct SessionId_t
{
    char sessionWord[36];
};


struct NickNameInfo
{
    int8_t nickNameLen;
    char nickName[MAX_NICK_NAME_LEN];
};


struct ChannelInfo
{
    int8_t channelNo;
    int8_t channelNameLen;
    char channelName[MAX_CHANNEL_NAME_LEN];
};


struct RoomInfo
{
    int16_t roomNo;
    int8_t roomNameLen;
    char roomName[MAX_ROOM_NAME_LEN];
};


struct RoomSlot
{
    int8_t isOpen;
    int8_t isExistUser;
    int8_t isReady;
    int8_t tribe;
    NickNameInfo nickNameInfo;
};


struct RoomDetailInfo
{
    int16_t roomNo;
    int8_t roomNameLen;
    char roomName[MAX_ROOM_NAME_LEN];
    int8_t mapType;
    int8_t masterIndex;
    RoomSlot roomSlotList[MAX_ROOM_SLOT_COUNT];
};


struct NickNameInfoWithOnline
{
    int8_t online;
    NickNameInfo nickNameInfo;
};

struct UserViewInfo
{
    int64_t userNo;
    NickNameInfo nickNameInfo;
};

struct RequestInfo
{
    int requestType;
    NickNameInfo nickNameInfo;
};


struct UserInfo
{
    int64_t userNo;
    int8_t nickNameLen;
    char nickName[MAX_NICK_NAME_LEN];
    int32_t gold;
    int8_t memoLen;
    char memo[MAX_MEMO_LEN];
    int16_t commonWin;
    int16_t commonLose;
    int16_t commonDiss;
    int16_t rankWin;
    int16_t rankLose;
    int16_t rankDiss;
    int8_t grade;
    int8_t gradeReachedCount;
    int8_t playCount;
};

struct UserInfoWithSessionId
{
    SessionId_t sessionId;
    UserInfo userInfo;
};

struct GameUserInfo
{
    int64_t userNo;
    int8_t tribe;
};

struct Packet
{
    command_t cmd;
};

#pragma pack(pop)


#endif //__CLIENT_LOBBY_PACKET_H__

