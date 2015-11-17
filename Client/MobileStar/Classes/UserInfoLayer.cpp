#include "UserInfoLayer.h"


bool UserInfoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("user_view_info.png");
//    bg->setContentSize(Size::Size(320, 72));
    bg->setAnchorPoint(Vec2(0.5, 1));
    
    this->addChild(bg, 1);
    
//    //디스패처. 리스너와 오브젝트를 연결해주는 역할
//    EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
//    //터치 위치를 알려주는 리스너. 단일 터치.
//    //바로 만들어쓰는 식별자는 auto를 사용한다.
//    auto positionListener = EventListenerTouchOneByOne::create();
//    //zOrder에 따라 밑에 깔린애도 동작할지 아닐지를 결정한다.
//    positionListener->setSwallowTouches(true);
//    //콜백 함수 대입
//    positionListener->onTouchBegan = CC_CALLBACK_2(UserInfoLayer::onTouchBegan, this);
//    //디스패처를 이용해 객체와 리스너를 이어준다. 화면 전체를 터치할 수 있게 만들어야 하므로 객체는 this
//    dispatcher->addEventListenerWithSceneGraphPriority(positionListener, bg);

    
//    
//    auto bgItem = MenuItemImage::create(
//                                      "user_view_info.png",
//                                      "user_view_info.png",
//                                      CC_CALLBACK_1(UserInfoLayer::clickMe, this));
//    bgItem->retain();
//    
//    auto menu = Menu::create(bgItem, NULL);
//    menu->setAnchorPoint(Vec2(0.5, 1));
//
//    this->addChild(menu, 19098900);

    return true;
}

UserInfoLayer::~UserInfoLayer()
{
    
}

void UserInfoLayer::SetUserInfo(int _nickNameLen, const char* _nickName)
{
    nickName = std::string(_nickName, _nickNameLen);
    
    auto nickNameLabel = Label::create(nickName, "fonts/arial.ttf", 48);
    nickNameLabel->setAnchorPoint(Vec2(0.5, 1));
    
    this->removeChildByTag(394);
    
    this->addChild(nickNameLabel, 2, 394);
}








void UserInfoLayer::clickMe(cocos2d::Ref* pSender)
{
    printf("fefefefefe\n");
}


//
//void UserListLayer::SetUserInfo(UserInfo* userInfo)
//{
//	DebugLog("userInfo");
//	((Menu*)this->getParent()->getChildByTag(TAG_MENU))->setEnabled(false);
//	this->addChild(userInfoLayer, 4020, TAG_USER_INFO_LAYER);
//}
//
//
//void UserListLayer::menuCloseCallback(Ref* pSender)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//	((Menu*)(this->getParent()->getChildByTag(TAG_MENU)))->setEnabled(true);
//	((LobbyScene*)(this->getParent()))->textBox->setEnabled(true);
//	this->removeFromParentAndCleanup(true);
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}
//
//void UserListLayer::userInfoLayerClose(Ref* pSender)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
//	return;
//#endif
//	((Menu*)(this->getChildByTag(TAG_MENU)))->setEnabled(true);
//	this->removeChildByTag(TAG_USER_INFO_LAYER, false);
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
//}
//
//void UserListLayer::doClick1(Object* pSender)
//{
//	((Menu*)(this->getChildByTag(TAG_MENU)))->setEnabled(false);
//
//}
//
//void UserListLayer::doClick2(Object* pSender)
//{
////	((Menu*)(this->getChildByTag(TAG_MENU)))->setEnabled(false);
////	auto findRoom = FindRoomLayer::create();
////	this->addChild(findRoom, 100, TAG_FIND_ROOM_LAYER);
//}
//
//void UserListLayer::doClick3(Object* pSender)
//{
//	((Menu*)(this->getChildByTag(TAG_MENU)))->setEnabled(false);
//
////	((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->SendRamdomJoinRoomReq();
//}
//
//void UserListLayer::ClickUserLabel(Object* pSender, LabelTTF* label)
//{
//	DebugLog("Touch %s %d", label->getString().c_str(), label->getString().length());
//	menu->setEnabled(false);
////	((NetworkLayer*)(Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER)))->SendGetUserInfoReq(label->getString().c_str(), label->getString().length());
//}