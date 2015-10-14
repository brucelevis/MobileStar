#include "LobbyScene.h"
//#include "BackGroundLayer.h"
#include "GameDefines.h"
#include "NetworkLayer.h"
#include "UserListLayer.h"
#include "GameClient.h"
#include "DefineHeader.h"
#include "MenuButtonLayer.h"
#include "MyInfoLayer.h"
#include "LobbyGameLayer.h"
#include "LobbyChannelLayer.h"
#include "LobbyFriendLayer.h"
#include "LobbyClanLayer.h"
#include "LobbyRankingLayer.h"
#include "LobbyBoardLayer.h"


USING_NS_CC;

Scene* LobbyScene::createScene()
{
	auto scene = Scene::create();

//	auto bgLayer = BackGroundLayer::create();
//
//	scene->addChild(bgLayer, 0);

	auto layer = LobbyScene::create();

	scene->addChild(layer, 1, TAG_LOBBY_SCENE);

	return scene;
}

bool LobbyScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    myInfoLayer = MyInfoLayer::create();
    myInfoLayer->retain();
    
    myInfoLayer->setAnchorPoint(Vec2(0, 0));
    myInfoLayer->setPosition(Vec2(0, MY_INFO_LAYER_Y));
    
    addChild(myInfoLayer, 0, TAG_MY_INFO_LAYER);
    
    
    menuButtonLayer = MenuButtonLayer::create();
    menuButtonLayer->retain();
    
    menuButtonLayer->setAnchorPoint(Vec2(0,0));
    menuButtonLayer->setPosition(Vec2(0,0));
    
    addChild(menuButtonLayer, 0, TAG_MENU_BUTTON_LAYER);
    
    
    lobbyGameLayer = LobbyGameLayer::create();
    lobbyGameLayer->retain();
    
    lobbyGameLayer->setAnchorPoint(Vec2(0, 0));
    lobbyGameLayer->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    lobbyChannelLayer = LobbyChannelLayer::create();
    lobbyChannelLayer->retain();
    
    lobbyChannelLayer->setAnchorPoint(Vec2(0, 0));
    lobbyChannelLayer->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    lobbyFriendLayer = LobbyFriendLayer::create();
    lobbyFriendLayer->retain();
    
    lobbyFriendLayer->setAnchorPoint(Vec2(0, 0));
    lobbyFriendLayer->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    lobbyClanLayer = LobbyClanLayer::create();
    lobbyClanLayer->retain();
    
    lobbyClanLayer->setAnchorPoint(Vec2(0, 0));
    lobbyClanLayer->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    lobbyRankingLayer = LobbyRankingLayer::create();
    lobbyRankingLayer->retain();
    
    lobbyRankingLayer->setAnchorPoint(Vec2(0, 0));
    lobbyRankingLayer->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    lobbyBoardLayer = LobbyBoardLayer::create();
    lobbyBoardLayer->retain();
    
    lobbyBoardLayer->setAnchorPoint(Vec2(0, 0));
    lobbyBoardLayer->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    
    
    
    
    
    addChild(lobbyGameLayer, 1, TAG_LOBBY_GAME_LAYER);
    
    currentLayer = CURRENT_LAYER_GAME;
    GameClient::GetInstance().currentScene = LOBBY_SCENE_NOW;
    
    
    
//	Size editboxSize = CCSizeMake(500, 70);
//	textBox = EditBox::create(editboxSize, Scale9Sprite::create("typefield_01.png"));
//	textBox->setAnchorPoint(ccp(0, 0));
//	textBox->setPosition(ccp(0, 0));
//	textBox->setFontSize(20);
//	textBox->setFontColor(ccWHITE);
//	textBox->setMaxLength(15);
//	textBox->setPlaceHolder("CHATTING");
//	textBox->setReturnType(EditBox::KeyboardReturnType::DONE);
//	textBox->setInputMode(EditBox::InputMode::ANY);
//	textBox->setDelegate(this);
//	this->addChild(textBox);


//    menuLayer = Layer::create();
//	menuLayer->setContentSize(Size(SCREEN_WIDTH / 4, SCREEN_HEIGHT));
//	menuLayer->setAnchorPoint(Vec2(0, 0));
//	menuLayer->setPosition(0, 0);
//    auto menuLayerbackGroundSprite = Sprite::create("back_ground_01.png");
//    menuLayerbackGroundSprite->setAnchorPoint(Vec2(0, 0));
//    menuLayerbackGroundSprite->setPosition(0, 0);
//    menuLayerbackGroundSprite->setScale((SCREEN_WIDTH / 4) / menuLayerbackGroundSprite->getContentSize().width, SCREEN_HEIGHT / menuLayerbackGroundSprite->getContentSize().height);
//    menuLayer->addChild(menuLayerbackGroundSprite, 0);
//    this->addChild(menuLayer, 100201);
//    
//    userListLayer = UserListLayer::create();
//    userListLayer->setAnchorPoint(Vec2(0, 0));
//    userListLayer->setPosition(Vec2(0, 0));
//    userListLayer->setContentSize(Size(SCREEN_WIDTH / 4, SCREEN_HEIGHT * 2));
//    
//    
//    scrollView = ScrollView::create(Size(SCREEN_WIDTH / 4, SCREEN_HEIGHT), userListLayer);
//    scrollView->retain();
//    scrollView->setBounceable(false);
//    scrollView->setDirection(ScrollView::Direction::VERTICAL);
//    scrollView->setAnchorPoint(Vec2(0, 0));
//    scrollView->setPosition(0, 0);
//    menuLayer->addChild(scrollView, 1);
//    scrollView->setContentOffset(Vec2(0, (-1 * userListLayer->getContentSize().height)), false);

/*
	auto moveChannel = MenuItemImage::create(
		"channel_list.png",
		"channel_list.png",
		CC_CALLBACK_1(LobbyScene::GetChannelList, this));

	moveChannel->setAnchorPoint(ccp(0, 0));
	moveChannel->setPosition(0, 1000);

	auto userList = MenuItemImage::create(
		"user_list.png",
		"user_list.png",
		CC_CALLBACK_1(LobbyScene::GetUserList, this));

	userList->setAnchorPoint(ccp(0, 0));
	userList->setPosition(0, 900);

	auto friendList = MenuItemImage::create(
		"friend_list.png",
		"friend_list.png",
		CC_CALLBACK_1(LobbyScene::GetFriendList, this));

	friendList->setAnchorPoint(ccp(0, 0));
	friendList->setPosition(0, 800);

	auto roomList = MenuItemImage::create(
		"room_list.png",
		"room_list.png",
		CC_CALLBACK_1(LobbyScene::GetRoomList, this));

	roomList->setAnchorPoint(ccp(0, 0));
	roomList->setPosition(0, 700);

	auto requestList = MenuItemImage::create(
		"request_list.png",
		"request_list.png",
		CC_CALLBACK_1(LobbyScene::GetRequestList, this));

	requestList->setAnchorPoint(ccp(0, 0));
	requestList->setPosition(0, 600);

	int reqCount = GameClient::GetInstance().requestInfoCount;
	std::string reqCountStr;
	GameClient::intToString(reqCountStr, reqCount);
	requestCountLabel = LabelTTF::create(reqCountStr, "Arial", 24);
	requestCountLabel->setColor(Color3B(0, 0, 0));
	requestCountLabel->setPosition(500, 650);
	menuLayer->addChild(requestCountLabel, 101);

	Menu* menu1 = Menu::create(moveChannel, userList, friendList, roomList, requestList, NULL);
	menu1->setAnchorPoint(ccp(0, 0));
	menu1->setPosition(ccp(0, 0));
	menuLayer->addChild(menu1, 100, TAG_MENU);


	channelListLayer = ChannelListLayer::create();
	channelListLayer->retain();

	userListLayer = UserListLayer::create();
	userListLayer->retain();

	roomListLayer = RoomListLayer::create();
	roomListLayer->retain();
	RoomInfo roomInfo[3];
	memset(roomInfo, 0, sizeof(roomInfo));
	for (int i = 0; i < 3; i++)
	{
		roomInfo[i].roomNo = i;
		memcpy(roomInfo[i].roomName, "room", sizeof(5));
	}
	roomListLayer->SetRoomList(3, roomInfo);


	requestListLayer = RequestListLayer::create();
	requestListLayer->retain();
	requestListLayer->SetRequestList();

*/
    
    
    
    
    
    EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
    //터치 위치를 알려주는 리스너. 단일 터치.
    //바로 만들어쓰는 식별자는 auto를 사용한다.
    auto positionListener = EventListenerTouchOneByOne::create();
    //zOrder에 따라 밑에 깔린애도 동작할지 아닐지를 결정한다.
    positionListener->setSwallowTouches(true);
    //콜백 함수 대입
    positionListener->onTouchBegan = CC_CALLBACK_2(LobbyScene::onTouchBegan, this);
    //디스패처를 이용해 객체와 리스너를 이어준다. 화면 전체를 터치할 수 있게 만들어야 하므로 객체는 this
    dispatcher->addEventListenerWithSceneGraphPriority(positionListener, this);

    
	return true;
}

void LobbyScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void LobbyScene::changeLobbyLayer(int layerNumber)
{
    if(currentLayer == layerNumber)
    {
        return ;
    }

    
    switch (currentLayer) {
        case CURRENT_LAYER_GAME:
        {
            removeChildByTag(TAG_LOBBY_GAME_LAYER, false);
            break;
        }
        case CURRENT_LAYER_CHANNEL:
        {
            removeChildByTag(TAG_LOBBY_CHANNEL_LAYER, false);
            break;
        }
        case CURRENT_LAYER_FRIEND:
        {
            removeChildByTag(TAG_LOBBY_FRIEND_LAYER, false);
            break;
        }
        case CURRENT_LAYER_CLAN:
        {
            removeChildByTag(TAG_LOBBY_CLAN_LAYER, false);

            break;
        }
        case CURRENT_LAYER_RANKING:
        {
            removeChildByTag(TAG_LOBBY_RANKING_LAYER, false);

            break;
        }
        case CURRENT_LAYER_BOARD:
        {
            removeChildByTag(TAG_LOBBY_BOARD_LAYER, false);

            break;
        }
            
        default:
            break;
    }

    
    switch (layerNumber) {
        case CURRENT_LAYER_GAME:
        {
            addChild(lobbyGameLayer, 1, TAG_LOBBY_GAME_LAYER);
            break;
        }
        case CURRENT_LAYER_CHANNEL:
        {
            addChild(lobbyChannelLayer, 1, TAG_LOBBY_CHANNEL_LAYER);
            break;
        }
        case CURRENT_LAYER_FRIEND:
        {
            addChild(lobbyFriendLayer, 1, TAG_LOBBY_FRIEND_LAYER);

            break;
        }
        case CURRENT_LAYER_CLAN:
        {
            addChild(lobbyClanLayer, 1, TAG_LOBBY_CLAN_LAYER);

            break;
        }
        case CURRENT_LAYER_RANKING:
        {
            addChild(lobbyRankingLayer, 1, TAG_LOBBY_RANKING_LAYER);

            break;
        }
        case CURRENT_LAYER_BOARD:
        {
            addChild(lobbyBoardLayer, 1, TAG_LOBBY_BOARD_LAYER);

            break;
        }
            
        default:
            break;
    }
    
    currentLayer = layerNumber;
}





void LobbyScene::addUserInfo(int64_t userNo, int nickNameLen, const char* nickName)
{
    lobbyChannelLayer->userListLayer->addUserViewInfo(userNo, nickNameLen, nickName);
}

void LobbyScene::removeUserInfo(int64_t userNo)
{
    lobbyChannelLayer->userListLayer->removeUserViewInfo(userNo);
}


void LobbyScene::waitToStart()
{
//    quickPlayButton->setSelectedSpriteFrame(SpriteFrame::create("quick_play_cancel_btn.png", Rect(0, 0, 400, 200)));

//    menu->setEnabled(false);
}





bool LobbyScene::onTouchBegan(Touch* touch, Event* _event){
    printf("onTouchBegan");

//    Rect userListRect = Rect(0,0,SCREEN_WIDTH / 4, SCREEN_HEIGHT);
//    Point point = Point(touch->getLocation().x, touch->getLocation().y);
//    if(userListRect.containsPoint(point))
//    {
//        printf("begin");
//        
//        Vec2 vec = this->convertToNodeSpace(Vec2(touch->getLocation().x, touch->getLocation().y));
//        
//        printf("begin %f %f\n", vec.x, vec.y);
//        
//        userListLayer->onTouchBegan(touch, _event);
//    }
    
    return true;
}
void LobbyScene::onTouchMoved(Touch* touch, Event* _event){
    
}
void LobbyScene::onTouchCancelled(Touch* touch, Event* _event){
    
}
void LobbyScene::onTouchEnded(Touch* touch, Event *_event){
    printf("end ");
}






/*
void LobbyScene::doClick1(Object* pSender)
{
	menu->setEnabled(false);
	textBox->setEnabled(false);

	auto moveLayer = MoveBy::create(0.2f, ccp(500, 0));
	menuLayer->runAction(moveLayer);

}
void LobbyScene::doClick2(Object* pSender)
{
	if (strcmp(textBox->getText(), "") == 0)
		return;

	std::string chattingStr = std::string(textBox->getText());
	if (chattingStr.at(0) == '/')
	{
		DebugLog("verify function call");
		if (chattingStr.length() < 5)
		{
			GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
			return;
		}

		if (chattingStr.at(1) == 'f')
		{
			if (chattingStr.at(2) == 'a')
			{
				if (chattingStr.at(3) == ' ')
				{
					std::string nickNameStr = chattingStr.substr(4);

					if (nickNameStr.length() != 0)
					{
					}
					else // πÆπ˝ø¿∑˘
					{
						GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
						return;
					}

				}
				else //πÆπ˝ ø¿∑˘
				{
					GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
					return;
				}
			}
			else if (chattingStr.at(2) == 'd')
			{
				if (chattingStr.at(3) == ' ')
				{
					std::string nickNameStr = chattingStr.substr(4);

					if (nickNameStr.length() != 0)
					{
					}
					else // πÆπ˝ø¿∑˘
					{
						GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
						return;
					}

				}
				else //πÆπ˝ ø¿∑˘
				{
					GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
					return;
				}
			}
			else//ƒ£±∏√ﬂ∞°ªË¡¶∞° æ∆¥“∂ß
			{
				GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
				return;
			}
		}
		else //ƒ£±∏ ±‚¥…¿Ã æ∆¥“∂ß
		{
			GameClient::GetInstance().customPopUp->PopUp("¿ﬂ∏¯µ» ø‰√ª¿‘¥œ¥Ÿ.");
			return;
		}
	}

//	((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->SendChattingReq(textBox->getText(), strlen(textBox->getText()));
	textBox->setText("");
}*/
//
//void LobbyScene::GetChannelList(Object* pSender)
//{
//	menuLayer->stopAllActions();
//	menuLayer->setPosition(ccp(-500, 0));
//
////	((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->lobbySendGetChannelListReq();
//
//}
//
//void LobbyScene::GetUserList(Object* pSender)
//{
//	menuLayer->stopAllActions();
//	menuLayer->setPosition(ccp(-500, 0));
//
////	((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->SendGetUserListReq();
//}
//
//void LobbyScene::GetFriendList(Object* pSender)
//{
//	menuLayer->stopAllActions();
//	menuLayer->setPosition(ccp(-500, 0));
//
////	((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->lobbySendGetRoomListReq();
//}
//
//void LobbyScene::GetRoomList(Object* pSender)
//{
//	menuLayer->stopAllActions();
//	menuLayer->setPosition(ccp(-500, 0));
//
//	this->addChild(roomListLayer, 1000002, TAG_ROOM_LIST_LAYER);
//}
//
//void LobbyScene::GetRequestList(Object* pSender)
//{
//	menuLayer->stopAllActions();
//	menuLayer->setPosition(ccp(-500, 0));
//
//	this->addChild(requestListLayer, 1000002, TAG_REQUEST_LIST_LAYER);
//}



//
//void LobbyScene::ChattingNotify(const char* nickName, const char* chatting, int chattingLen)
//{
//	std::string text = "[";
//	text.append(nickName);
//	text.append("] : ");
//	text.append(chatting);
//	auto chattingText = LabelTTF::create(text, "Arial", 24);
//	chattingText->setAnchorPoint(ccp(0, 0));
//	chattingText->setPosition(ccp(0, 0));
//
//	for (int i = 0; i < MAX_CHATTING_COUNT; i++)
//	{
//		LabelTTF* lt = ((LabelTTF*)scrollContainer->getChildByTag(1000 + i));
//		if (lt == NULL)
//		{
//			break;
//		}
//		lt->setPosition(ccp(0, 30+ (i * 30)));
//
//	}
//
//	LabelTTF* lt1 = ((LabelTTF*)scrollContainer->getChildByTag(1000 + MAX_CHATTING_COUNT - 1));
//	if (lt1 != NULL)
//	{
//		lt1->release();
//	}
//
//	for (int i = MAX_CHATTING_COUNT -1; i >=0; i--)
//	{
//		LabelTTF* lt = ((LabelTTF*)scrollContainer->getChildByTag(1000 + i));
//		if (lt == NULL)
//		{
//			continue;
//		}
//
//		lt->setTag(1000 + i + 1);
//	}
//
//	scrollContainer->addChild(chattingText, 0, 1000);
//}
//
//void LobbyScene::SetChannelListLayer(int channelCount, ChannelInfo * channelInfo)
//{
//	channelListLayer->SetChannelList(channelCount, channelInfo);
//	this->addChild(channelListLayer, 1000002, TAG_CHANNEL_LIST_LAYER);
//}
//
//void LobbyScene::SetRoomListLayer(int roomCount, RoomInfo * roomInfo)
//{
//    roomListLayer->SetRoomList(roomCount, roomInfo);
//    this->addChild(channelListLayer, 1000002, TAG_ROOM_LIST_LAYER);
//}
//
//void LobbyScene::SetUserListLayer(int nickNameInfoCount, NickNameInfo* nickNameInfoList)
//{
////	userListLayer->SetUserList(nickNameInfoCount, nickNameInfoList);
//	this->addChild(userListLayer, 1000002, TAG_USER_LIST_LAYER);
//}
//
//void LobbyScene::MoveChannel(int channelNo, const char* channelName)
//{
//	this->removeChildByTag(TAG_CHANNEL_LIST_LAYER, false);
//	scrollContainer->removeAllChildren();
//	menu->setEnabled(true);
//	textBox->setEnabled(true);
//}
//
//void LobbyScene::CloseMenu()
//{
//	this->removeChildByTag(TAG_CHANNEL_LIST_LAYER, false);
//	menu->setEnabled(true);
//	textBox->setEnabled(true);
//}
//
//void LobbyScene::AddRequestInfo(int requestType, int nickNameLen, const char* nickName)
//{
//	GameClient::GetInstance().requestInfoCount++;
//	GameClient::GetInstance().SetRequestInfo(requestType, nickNameLen, nickName);
//
//	requestListLayer->AddRequestInfo(GameClient::GetInstance().requestInfoList.at(GameClient::GetInstance().requestInfoList.size() - 1));
//
//
//	int reqCount = GameClient::GetInstance().requestInfoCount;
//	std::string reqCountStr;
//	GameClient::intToString(reqCountStr, reqCount);
//	requestCountLabel->setString(reqCountStr);
//}
//
//void LobbyScene::RemoveRequestInfo()
//{
//	int reqNo = requestListLayer->selectedReqNo;
//
//	GameClient::GetInstance().requestInfoCount--;
//	RequestInfo* requestInfo = GameClient::GetInstance().requestInfoList.at(reqNo);
//
//	GameClient::GetInstance().requestInfoList.erase(GameClient::GetInstance().requestInfoList.begin() + reqNo);
//	delete requestInfo;
//
//	requestListLayer->SetRequestList();
//
//	requestListLayer->selectedReqNo = -1;
//
//	int reqCount = GameClient::GetInstance().requestInfoCount;
//	std::string reqCountStr;
//	GameClient::intToString(reqCountStr, reqCount);
//	requestCountLabel->setString(reqCountStr);
//}