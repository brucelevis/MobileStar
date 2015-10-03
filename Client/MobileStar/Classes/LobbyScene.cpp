#include "LobbyScene.h"
//#include "BackGroundLayer.h"
#include "GameDefines.h"
#include "NetworkLayer.h"
#include "UserListLayer.h"
#include "GameClient.h"
#include "DefineHeader.h"

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

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(LobbyScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

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

    tribeChina = MenuItemImage::create(
                                            "china_btn.png",
                                            "china_btn.png",
                                            CC_CALLBACK_1(LobbyScene::clickChina, this));
    
    tribeEurope = MenuItemImage::create(
                                            "europe_btn.png",
                                            "europe_btn.png",
                                            CC_CALLBACK_1(LobbyScene::clickEurope, this));
    
    tribeUSA = MenuItemImage::create(
                                          "usa_btn.png",
                                          "usa_btn.png",
                                          CC_CALLBACK_1(LobbyScene::clickUSA, this));
    
    tribeRandom = MenuItemImage::create(
                                          "random_btn.png",
                                          "random_btn.png",
                                          CC_CALLBACK_1(LobbyScene::clickRandom, this));
    
    
    
    quickPlayButton = MenuItemImage::create(
		"quick_play_btn.png",
		"quick_play_btn.png",
		CC_CALLBACK_1(LobbyScene::clickQuickPlay, this));

    tribeChina->setPosition((SCREEN_WIDTH / 4) + (3 * (SCREEN_WIDTH / 32)), (2 * SCREEN_HEIGHT / 3) + (3 * SCREEN_HEIGHT / 12));
    tribeEurope->setPosition((SCREEN_WIDTH / 4) + 3 * (3 * (SCREEN_WIDTH / 32)), (2 * SCREEN_HEIGHT / 3) + (3 * (SCREEN_HEIGHT / 12)));
    tribeUSA->setPosition((SCREEN_WIDTH / 4) + (3 * (SCREEN_WIDTH / 32)), (2 * SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 12));
    tribeRandom->setPosition((SCREEN_WIDTH / 4) + 3 * (3 * (SCREEN_WIDTH / 32)), (2 * SCREEN_HEIGHT / 3) + (SCREEN_HEIGHT / 12));
    
    quickPlayButton->setPosition((SCREEN_WIDTH / 4) + 6 * (3 * (SCREEN_WIDTH / 32)), (2 * SCREEN_HEIGHT / 3) + (2 * SCREEN_HEIGHT / 12));

	menu = Menu::create(tribeChina, tribeEurope, tribeUSA, tribeRandom, quickPlayButton, NULL);
    menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 100, TAG_MENU);
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_CHINA;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeChina->addChild(choose, 0, 123);
    
    
    
	menuLayer = Layer::create();
	menuLayer->setContentSize(Size(SCREEN_WIDTH / 4, SCREEN_HEIGHT));
	menuLayer->setAnchorPoint(Vec2(0, 0));
	menuLayer->setPosition(0, 0);
    auto menuLayerbackGroundSprite = Sprite::create("back_ground_01.png");
    menuLayerbackGroundSprite->setAnchorPoint(Vec2(0, 0));
    menuLayerbackGroundSprite->setPosition(0, 0);
    menuLayerbackGroundSprite->setScale((SCREEN_WIDTH / 4) / menuLayerbackGroundSprite->getContentSize().width, SCREEN_HEIGHT / menuLayerbackGroundSprite->getContentSize().height);
    menuLayer->addChild(menuLayerbackGroundSprite, 0);
    this->addChild(menuLayer, 100201);

    // Setup scroll container
    scrollContainer = Layer::create();
    /*
     char menuName[20];
     for (int i = 0; i < MAX_CHATTING_COUNT; i++)
     {
     snprintf(menuName, sizeof(menuName), "ITEM %d", i);
     Node* menuLabel = CCLabelTTF::create(menuName, "Arial", 24);
     Size menuSize = menuLabel->getContentSize();
     
     MenuItem* menuItem = MenuItemLabel::create(menuLabel);
     menuItem->setTag(BASE_TAG + i);
     menuItem->setPosition(ccp(0, i * 100));
     
     scrollContainer->addChild(menuItem);
     }*/
    
    userListLayer = UserListLayer::create();
    userListLayer->setAnchorPoint(Vec2(0, 0));
    userListLayer->setPosition(Vec2(0, 0));
    userListLayer->setContentSize(Size(SCREEN_WIDTH / 4, 1000000));
    
    
    scrollView = ScrollView::create(Size(SCREEN_WIDTH / 4, SCREEN_HEIGHT), userListLayer);    // ¿ßø°º≠ ¡§¿««— container∏¶ ªÁøÎ
    scrollView->retain();
    scrollView->setBounceable(false);        // »≠∏È¿« ≥°±Ó¡ˆ Ω∫≈©∑—«ﬂ¿ª ∂ß ∆®±Ë ø©∫Œ∏¶ º≥¡§, ±‚∫ª∞™¿∫ True.
    scrollView->setDirection(ScrollView::Direction::VERTICAL);    // Scroll µ«¥¬ πÊ«‚¿ª º≥¡§ (Vertical/Horizontal)
    scrollView->setAnchorPoint(Vec2(0, 0));
    scrollView->setPosition(0, 0);
    menuLayer->addChild(scrollView, 1);
    scrollView->setContentOffset(Vec2(0, (-1 * userListLayer->getContentSize().height)), false);

//    scrollContainer->addChild(<#cocos2d::Node *child#>)

    //	scrollView->setDelegate(scrollViewDelegateLayer);             // ScrollView∏¶ Delegate«“ Layer∑Œ ¿ßø°º≠ ¡§¿««— ScrollViewDelegateLayer∏¶ ¡ˆ¡§«ÿ¡›¥œ¥Ÿ.
    
    // offset¿Ã∂ı »≠∏È¿« Ω√¿€¡ˆ¡°¿∏∑Œ∫Œ≈Õ ∂≥æÓ¡¯ ∞≈∏Æ∏¶ ¿«πÃ«œ¥¬µ•,
    // ±‚∫ª∞™¿Ã 0¿Ã±‚ ∂ßπÆø° ºº∑Œ ScrollView¿« ∞ÊøÏ ∞°¿Â æ∆∑°∑Œ ¡§«ÿ¡Æ ¿÷Ω¿¥œ¥Ÿ.
    // √≥¿Ω ∫∏ø©¡ˆ¥¬ Contrainer¿« ∫Œ∫–¿ª ∞°¿Â ¿ß∑Œ πŸ≤Ÿ±‚ ¿ß«ÿº≠¥¬ Container¿« ≈©±‚∏¶ ¿Ωºˆ∑Œ ¡÷Ω√∏È µÀ¥œ¥Ÿ.
    //	scrollView->setContentOffset(Point(0, -1 * container->getContentSize().height), false);

    
    for(int i = 0; i  < 20; i++)
    addNextUserList(i, 5, "test1");
    
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


void LobbyScene::addNextUserList(int64_t userNo, int nickNameLen, const char* nickName)
{
    userListLayer->addNextUserList(userNo, nickNameLen, nickName);
}

void LobbyScene::addPrevUserList(int64_t userNo, int nickNameLen, const char* nickName)
{

}



void LobbyScene::clickChina(cocos2d::Ref *pSender)
{
    int tribe = GameClient::GetInstance().tribe;
    
    if(tribe == TRIBE_TYPE_CHINA)
    {
        tribeChina->removeChildByTag(123);
    }
    else if(tribe == TRIBE_TYPE_EUROPE)
    {
        tribeEurope->removeChildByTag(123);
    }
    else if(tribe == TRIBE_TYPE_USA)
    {
        tribeUSA->removeChildByTag(123);

    }
    else
    {
        tribeRandom->removeChildByTag(123);

    }
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_CHINA;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeChina->addChild(choose, 1, 123);
}
void LobbyScene::clickEurope(cocos2d::Ref *pSender)
{
    int tribe = GameClient::GetInstance().tribe;

    if(tribe == TRIBE_TYPE_CHINA)
    {
        tribeChina->removeChildByTag(123);

    }
    else if(tribe == TRIBE_TYPE_EUROPE)
    {
        tribeEurope->removeChildByTag(123);

    }
    else if(tribe == TRIBE_TYPE_USA)
    {
        tribeUSA->removeChildByTag(123);

    }
    else
    {
        tribeRandom->removeChildByTag(123);

    }
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_EUROPE;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeEurope->addChild(choose, 1, 123);
}
void LobbyScene::clickUSA(cocos2d::Ref *pSender)
{
    int tribe = GameClient::GetInstance().tribe;
    
    if(tribe == TRIBE_TYPE_CHINA)
    {
        tribeChina->removeChildByTag(123);

    }
    else if(tribe == TRIBE_TYPE_EUROPE)
    {
        tribeEurope->removeChildByTag(123);

    }
    else if(tribe == TRIBE_TYPE_USA)
    {
        tribeUSA->removeChildByTag(123);
    }
    else
    {
        tribeRandom->removeChildByTag(123);

    }
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_USA;
    tribeUSA->addChild(choose, 1, 123);
}
void LobbyScene::clickRandom(cocos2d::Ref *pSender)
{
    int tribe = GameClient::GetInstance().tribe;
    
    if(tribe == TRIBE_TYPE_CHINA)
    {
        tribeChina->removeChildByTag(123);

    }
    else if(tribe == TRIBE_TYPE_EUROPE)
    {
        tribeEurope->removeChildByTag(123);

    }
    else if(tribe == TRIBE_TYPE_USA)
    {
        tribeUSA->removeChildByTag(123);

    }
    else
    {
        tribeRandom->removeChildByTag(123);

    }
    
    GameClient::GetInstance().tribe = -1;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeRandom->addChild(choose, 1, 123);
    
}



void LobbyScene::clickQuickPlay(cocos2d::Ref *pSender)
{
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->lobbySendQuickPlayReq(GameClient::GetInstance().tribe);
}


void LobbyScene::waitToStart()
{
    quickPlayButton->setSelectedSpriteFrame(SpriteFrame::create("quick_play_cancel_btn.png", Rect(0, 0, 400, 200)));

    menu->setEnabled(false);
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