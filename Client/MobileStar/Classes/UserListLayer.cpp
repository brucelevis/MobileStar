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
    if ( !Layer::init() )
    {
        return false;
    }
	

    userCount = 0;
    nextAddUserNumber = 0;
    downCount = 0;
    
 //   MenuItemImage* bgItem[MAX_USER_COUNT_IN_CHANNEL];
    
    for(int i = 0; i < MAX_USER_COUNT_IN_CHANNEL; i++)
    {
        UserInfoLayer * userInfoLayer = UserInfoLayer::create();
        userInfoLayer->retain();
        userInfoLayer->SetUserInfo(INVALID_USER_NO, 0, NULL);
        userInfoLayer->setAnchorPoint(Vec2(0, 0));
        userInfoLayerList.push_back(userInfoLayer);
        
        
        
        
//        
//        bgItem[i] = MenuItemImage::create(
//                                           "user_view_info.png",
//                                           "user_view_info.png",
//                                           CC_CALLBACK_1(UserListLayer::clickUserInfo, this));
//        bgItem[i]->retain();
//        
//        bgItem[i]->setPosition(Vec2(getContentSize().width / 2, getContentSize().height - (nextAddUserNumber+1) * (SCREEN_HEIGHT / 10)));
    }
    
//    menu = Menu::create(bgItem[0], bgItem[1], bgItem[2], NULL);
//    menu->setAnchorPoint(Vec2::ZERO);
//    menu->setPosition(Vec2::ZERO);
    
//    this->addChild(menu, 100, TAG_MENU);

    
    return true;
}

void UserListLayer::addUserViewInfo(int64_t userNo, int nickNameLen, const char* nickName)
{
    UserInfoLayer * userInfoLayer = userInfoLayerList.front();
    userInfoLayerList.erase(userInfoLayerList.begin());
    userInfoLayer->SetUserInfo(userNo, nickNameLen, nickName);
//    userInfoLayer->setScale(((float)SCREEN_WIDTH / 4) / SCREEN_WIDTH, ((float)SCREEN_HEIGHT / 10) / SCREEN_HEIGHT);
    userInfoLayer->setPosition(Vec2(getContentSize().width / 2, getContentSize().height - (nextAddUserNumber) * (SCREEN_HEIGHT / 10)));
    
    userInfoLayer->layerPosition = nextAddUserNumber;
    //    MenuItem* item = MenuItem::create(userInfoLayerList[nextAddUserNumber], CC_CALLBACK_1(UserListLayer::ClickUserInfo, this, nextAddUserNumber));
    //    item->setAnchorPoint(ccp(0, 0));
    //    item->setPosition(0, 1000 - i * 100);
    
    //    DebugLog("%s", nickNameInfo[i].nickName);
    //
    //    menu->addChild(item);
    
//    this->removeChildByTag(nextAddUserNumber);
    
    
    this->addChild(userInfoLayer, 1, (int)userNo);
    
    nextAddUserNumber++;
}

void UserListLayer::removeUserViewInfo(int64_t userNo)
{
    std::vector<UserInfoLayer*>::iterator itr = userInfoLayerList.begin();
    
    UserInfoLayer* userInfoLayer = (UserInfoLayer*)getChildByTag((int)userNo);
    if(userInfoLayer->userNo == userNo)
    {
        userInfoLayerList.push_back(userInfoLayer);
        removeChild(userInfoLayer);
    }
    
    nextAddUserNumber--;
    
    const Vector<Node*> childVector = getChildren();
    
    for(int i = 0; i < nextAddUserNumber; i++)
    {
        ((UserInfoLayer*)childVector.at(i))->setPosition(Vec2(getContentSize().width / 2, getContentSize().height - i * (SCREEN_HEIGHT / 10)));
        ((UserInfoLayer*)childVector.at(i))->layerPosition = i;
    }
}


void UserListLayer::clickUserInfo(cocos2d::Ref *pSender)
{
    printf("click user Info %d\n", 1);
}



bool UserListLayer::onTouchBegan(Touch* touch, Event* _event){
    
    Vec2 vec = this->convertToNodeSpace(Vec2(touch->getLocation().x, touch->getLocation().y));
    
    printf("begin %f %f\n", vec.x, vec.y);
    
    int position = 19 - ((int)(vec.y / (SCREEN_HEIGHT / 10)));
    
    if(position >= nextAddUserNumber)
    {
        printf("??");
        return false;
    }
    
    
    const Vector<Node*> childVector = getChildren();
    
    
    
    UserInfoLayer* userInfoLayer = ((UserInfoLayer*)childVector.at(position));
    
    printf("%lld", userInfoLayer->userNo);

    return true;
}
void UserListLayer::onTouchMoved(Touch* touch, Event* _event){
    
}
void UserListLayer::onTouchCancelled(Touch* touch, Event* _event){
    
}
void UserListLayer::onTouchEnded(Touch* touch, Event *_event){
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