#include "MyInfoLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "BasicDefines.h"

USING_NS_CC;


bool MyInfoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto gameBtn = MenuItemImage::create(
                                         "menu_game_btn_01.png",
                                         "menu_game_btn_02.png",
                                         CC_CALLBACK_1(MyInfoLayer::clickGameBtn, this));
    
    gameBtn->setAnchorPoint(Vec2(0,0));
    gameBtn->setPosition(0, 0);
   
	menu = Menu::create(gameBtn, NULL);
	menu->setPosition(Vec2::ZERO);

    return true;
}

void MyInfoLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void MyInfoLayer::clickGameBtn(cocos2d::Ref* pSender)
{
//    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendFirstConnectReq();
    
}