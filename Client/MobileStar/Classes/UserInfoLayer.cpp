#include "UserInfoLayer.h"

USING_NS_CC;

bool UserInfoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

UserInfoLayer::~UserInfoLayer()
{
    
}

void UserInfoLayer::SetUserInfo(int64_t _userNo, int _nickNameLen, const char* _nickName)
{
    userNo = _userNo;
    nickName = std::string(_nickName, _nickNameLen);
    
    auto nickNameLabel = Label::create(nickName, "Arial", 24);
    this->removeAllChildren();
    this->addChild(nickNameLabel, 1);
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