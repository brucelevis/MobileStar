#ifndef __LOBBY_SCENE_H__
#define __LOBBY_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

#include "BasicPacket.h"
#include "UserListLayer.h"

class LobbyGameLayer;
class LobbyChannelLayer;
class LobbyFriendLayer;
class LobbyClanLayer;
class LobbyRankingLayer;
class LobbyBoardLayer;


class ChannelListLayer;
class ChannelInfo;
class UserListLayer;
class RoomListLayer;
class RoomInfo;
class RequestListLayer;
class NickNameInfo;

const int MAX_CHATTING_COUNT = 30;

enum CURRENT_LAYER
{
    CURRENT_LAYER_GAME = 0,
    CURRENT_LAYER_CHANNEL,
    CURRENT_LAYER_FRIEND,
    CURRENT_LAYER_CLAN,
    CURRENT_LAYER_RANKING,
    CURRENT_LAYER_BOARD,
    
    CURRENT_LAYER_COUNT,
};

class LobbyScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);
    
    
    void changeLobbyLayer(int layerNumber);
    
    
    
    
    
    
    void addUserInfo(int nickNameLen, const char* nickName);
    void removeUserInfo(int nickNameLen, const char* nickName);
    
    void waitToStart();

    virtual bool onTouchBegan(Touch* touch, Event* _event);
    virtual void onTouchMoved(Touch* touch, Event* _event);
    virtual void onTouchCancelled(Touch* touch, Event* _event);
    virtual void onTouchEnded(Touch* touch, Event *_event);
//    void GetChannelList(cocos2d::Object* pSender);
//	void GetUserList(cocos2d::Object* pSender);
//	void GetFriendList(cocos2d::Object* pSender);
//	void GetRoomList(cocos2d::Object* pSender);
//	void GetRequestList(cocos2d::Object* pSender);

	CREATE_FUNC(LobbyScene);

//	void ChattingNotify(const char* nickName, const char* chatting, int chattingLen);
//    void SetChannelListLayer(int channelCount, ChannelInfo * channelInfo);
//    void SetRoomListLayer(int roomCount, RoomInfo * roomInfo);
//	void SetUserListLayer(int nickNameInfoCount, NickNameInfo* nickNameInfoList);
//	void AddRequestInfo(int requestType, int nickNameLen, const char* nickName);
//	void RemoveRequestInfo();
//	void MoveChannel(int channelNo, const char* channelName);
//	void CloseMenu();

//private:
	Menu* menu;
	Layer* scrollContainer;
//	ScrollView* scrollView;
	Layer* menuLayer;
	ChannelListLayer* channelListLayer;
	UserListLayer* userListLayer;
	RoomListLayer* roomListLayer;
	RequestListLayer* requestListLayer;
    
    MenuItemImage* tribeChina;
    MenuItemImage* tribeEurope;
    MenuItemImage* tribeUSA;
    MenuItemImage* tribeRandom;
    MenuItemImage* quickPlayButton;
    
    Layer* myInfoLayer;
    
    LobbyGameLayer* lobbyGameLayer;
    LobbyChannelLayer* lobbyChannelLayer;
    LobbyFriendLayer* lobbyFriendLayer;
    LobbyClanLayer* lobbyClanLayer;
    LobbyRankingLayer* lobbyRankingLayer;
    LobbyBoardLayer* lobbyBoardLayer;
    
    Layer* menuButtonLayer;
    
    int currentLayer;
    
    Sprite* chooseTribe;
};

#endif 

