#include "LoginScene.h"
//#include "BackGroundLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "DefineHeader.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();

	auto layer = LoginScene::create();
	
	scene->addChild(layer, 1, TAG_LOGIN_SCENE);

    return scene;
}

bool LoginScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto loginBtn = MenuItemImage::create(
		"btn_makeroom_01.png",
		"btn_makeroom_01.png",
		CC_CALLBACK_1(LoginScene::clickLoginBtn, this));

	loginBtn->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(LoginScene::menuCloseCallback, this));
    
    
	auto menu = Menu::create(loginBtn, closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 100, TAG_MENU);

    return true;
}

void LoginScene::menuCloseCallback(Ref* pSender)
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

void LoginScene::clickLoginBtn(Ref* pSender)
{
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendFirstConnectReq();

}