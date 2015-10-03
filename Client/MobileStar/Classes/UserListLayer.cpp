#include "UserListLayer.h"
#include "GameClient.h"
#include "NetworkLayer.h"
#include "GameDefines.h"
#include "LobbyScene.h"
#include "DefineHeader.h"
#include <stdio.h>
USING_NS_CC;

bool UserListLayer::init()
{
	Size size = Director::sharedDirector()->getWinSize();
    if ( !Layer::init() )
    {
        return false;
    }
	
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//	backgroundImage = Sprite::create("background_01.png");
//	backgroundImage->setAnchorPoint(ccp(0, 0));
//	backgroundImage->setPosition(0, 0);
//	this->addChild(backgroundImage, 0);
//
//	closeItem = MenuItemImage::create(
//                                           "btn_x_01.png",
//                                           "btn_x_01.png",
//										   CC_CALLBACK_1(UserListLayer::menuCloseCallback, this));
//
//	closeItem->setPosition(550, 1030);
//
//    menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//	this->addChild(menu, 100, TAG_MENU);

    userCount = 0;
    nextAddUserNumber = 0;
    downCount = 0;
    
    for(int i = 0; i < MAX_USER_COUNT_IN_CHANNEL + 10; i++)
    {
        userInfoLayerList[i] = UserInfoLayer::create();
        userInfoLayerList[i]->retain();
        userInfoLayerList[i]->SetUserInfo(INVALID_USER_NO, 0, NULL);
        
    }
    
    return true;
}


void UserListLayer::addNextUserList(int64_t userNo, int nickNameLen, const char* nickName)
{
    userInfoLayerList[nextAddUserNumber]->SetUserInfo(userNo, nickNameLen, nickName);
    userInfoLayerList[nextAddUserNumber]->setAnchorPoint(Vec2(0, 1));
    
    userInfoLayerList[nextAddUserNumber]->setPosition(Vec2(getContentSize().width / 2,  getContentSize().height - (downCount * (SCREEN_HEIGHT / 10))));
//    MenuItem* item = MenuItem::create(userInfoLayerList[nextAddUserNumber], CC_CALLBACK_1(UserListLayer::ClickUserInfo, this, nextAddUserNumber));
//    item->setAnchorPoint(ccp(0, 0));
//    item->setPosition(0, 1000 - i * 100);
    
//    DebugLog("%s", nickNameInfo[i].nickName);
//    
//    menu->addChild(item);
    
    this->removeChildByTag(nextAddUserNumber);
    
    this->addChild(userInfoLayerList[nextAddUserNumber], 1, nextAddUserNumber);

    nextAddUserNumber++;
    
    if(nextAddUserNumber >= MAX_USER_COUNT_IN_CHANNEL + 10)
    {
        nextAddUserNumber = 0;
    }
    
    downCount++;
}


void UserListLayer::addPrevUserList(int64_t userNo, int nickNameLen, const char* nickName)
{
    
}



void UserListLayer::ClickUserInfo(cocos2d::Ref *pSender)
{
    printf("click user Info %d\n", 1);
}














//
//
//void UserListLayer::SetUserInfo(UserInfo* userInfo)
//{
//	DebugLog("userInfo");
//	((Menu*)this->getParent()->getChildByTag(TAG_MENU))->setEnabled(false);
//	this->addChild(userInfoLayer, 4020, TAG_USER_INFO_LAYER);
//}
//
//
//
//
//
//
//
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