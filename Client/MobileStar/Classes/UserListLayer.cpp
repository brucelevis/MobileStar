#include "UserListLayer.h"
#include "GameClient.h"
#include "NetworkLayer.h"
#include "GameDefines.h"
#include "LobbyScene.h"
#include "DefineHeader.h"
#include <stdio.h>
USING_NS_CC;
using namespace ui;

bool UserListLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
    
    auto closeBtn = MenuItemImage::create(
                                            "cancel_btn_01.png",
                                            "cancel_btn_01.png",
                                            CC_CALLBACK_1(UserListLayer::clickCloseBtn, this));
    
    closeBtn->setAnchorPoint(Vec2(0, 0));
    closeBtn->setPosition(Vec2(DISPLAY_WIDTH - 100, DISPLAY_HEIGHT - 100));
    
    menu = Menu::create(closeBtn, NULL);
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    
    this->addChild(menu, 100, TAG_MENU);
    
    

    userCount = 0;
    nextAddUserNumber = 0;
    downCount = 0;
    
    
    for(int i = 0; i < MAX_USER_COUNT_IN_CHANNEL; i++)
    {
        UserInfoLayer * userInfoLayer = UserInfoLayer::create();
        userInfoLayer->retain();
        userInfoLayer->SetUserInfo(0, NULL);
        userInfoLayer->setAnchorPoint(Vec2(0, 0));
        userInfoLayerList.push_back(userInfoLayer);
    }

    
    scrollView = cocos2d::ui::ScrollView::create();
    scrollView->retain();
    
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setInnerContainerSize(Size(DISPLAY_WIDTH, CONTAINER_HEIGHT));
    scrollView->setContentSize(Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
    scrollView->setBounceEnabled(false);
    scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    
    scrollView->setBackGroundImage("user_list_back_ground.png");
    scrollView->setPosition(Vec2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2));
    
    this->addChild(scrollView, 99);
    
    return true;
}

void UserListLayer::addUserViewInfo(int nickNameLen, const char* nickName)
{
    UserInfoLayer * userInfoLayer = userInfoLayerList.front();
    userInfoLayerList.erase(userInfoLayerList.begin());
    userInfoLayer->SetUserInfo(nickNameLen, nickName);
//    userInfoLayer->setScale(((float)SCREEN_WIDTH / 4) / SCREEN_WIDTH, ((float)SCREEN_HEIGHT / 10) / SCREEN_HEIGHT);
    userInfoLayer->setPosition(Vec2(DISPLAY_WIDTH / 2, CONTAINER_HEIGHT - 128 - (nextAddUserNumber * (SCREEN_HEIGHT / 10))));
    
//        userInfoLayer->setPosition(Vec2(getContentSize().width / 2, getContentSize().height - (nextAddUserNumber) * (SCREEN_HEIGHT / 10)));
    
    userInfoLayer->layerPosition = nextAddUserNumber;
    //    MenuItem* item = MenuItem::create(userInfoLayerList[nextAddUserNumber], CC_CALLBACK_1(UserListLayer::ClickUserInfo, this, nextAddUserNumber));
    //    item->setAnchorPoint(ccp(0, 0));
    //    item->setPosition(0, 1000 - i * 100);
    
    //    DebugLog("%s", nickNameInfo[i].nickName);
    //
    //    menu->addChild(item);
    
//    this->removeChildByTag(nextAddUserNumber);
    
    scrollView->addChild(userInfoLayer, 1);
//    this->addChild(userInfoLayer, 1, (int)userNo);
    
    nextAddUserNumber++;
}

void UserListLayer::removeUserViewInfo(int nickNameLen, const char* nickName)
{
    std::vector<UserInfoLayer*>::iterator itr = userInfoLayerList.begin();
    
//    UserInfoLayer* userInfoLayer = (UserInfoLayer*)scrollView->getChildByTag((int)userNo);
//    if(userInfoLayer->userNo == userNo)
//    {
//        userInfoLayerList.push_back(userInfoLayer);
//        scrollView->removeChild(userInfoLayer);
//    }
//    
//    nextAddUserNumber--;
//    
//    const Vector<Node*> childVector = scrollView->getChildren();
//    
//    for(int i = 0; i < nextAddUserNumber; i++)
//    {
//        ((UserInfoLayer*)childVector.at(i))->setPosition(Vec2(DISPLAY_WIDTH / 2, CONTAINER_HEIGHT - 128 - (i * (SCREEN_HEIGHT / 10))));
//        ((UserInfoLayer*)childVector.at(i))->layerPosition = i;
//    }
}


void UserListLayer::clickUserInfo(cocos2d::Ref *pSender)
{
    printf("click user Info %d\n", 1);
}

void UserListLayer::clickCloseBtn(cocos2d::Ref* pSender)
{
    getParent()->removeChildByTag(TAG_USER_LIST_LAYER, false);
}


bool UserListLayer::onTouchBegan(Touch* touch, Event* _event){
    
    Vec2 vec = scrollView->convertToNodeSpace(Vec2(touch->getLocation().x, touch->getLocation().y));
    
    printf("begin %f %f\n", vec.x, vec.y);
    
    int position = 19 - ((int)(vec.y / (SCREEN_HEIGHT / 10)));
    
    if(position >= nextAddUserNumber)
    {
        printf("??");
        return false;
    }
    
    
    const Vector<Node*> childVector = scrollView->getChildren();
    
    
    
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