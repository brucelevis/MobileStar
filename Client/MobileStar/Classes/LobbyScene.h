#ifndef __LOBBY_SCENE_H__
#define __LOBBY_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

#include "BasicPacket.h"
#include "UserListLayer.h"



class ChannelListLayer;
class ChannelInfo;
class UserListLayer;
class RoomListLayer;
class RoomInfo;
class RequestListLayer;
class NickNameInfo;

const int MAX_CHATTING_COUNT = 30;

class LobbyScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);
    void addNextUserList(int64_t userNo, int nickNameLen, const char* nickName);
    void addPrevUserList(int64_t userNo, int nickNameLen, const char* nickName);

    void clickChina(cocos2d::Ref* pSender);
    void clickEurope(cocos2d::Ref* pSender);
    void clickUSA(cocos2d::Ref* pSender);
    void clickRandom(cocos2d::Ref* pSender);
    
    void clickQuickPlay(cocos2d::Ref* pSender);
    
    void waitToStart();

    
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
	ScrollView* scrollView;
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
        
    Sprite* chooseTribe;
};

#endif 

