#include "ResultScene.h"
//#include "BackGroundLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "DefineHeader.h"
#include "GameClient.h"

USING_NS_CC;

Scene* ResultScene::createScene()
{
    auto scene = Scene::create();

	auto layer = ResultScene::create();
	
	scene->addChild(layer, 1, TAG_RESULT_SCENE);

    return scene;
}

bool ResultScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto loginBtn = MenuItemImage::create(
		"btn_makeroom_01.png",
		"btn_makeroom_01.png",
		CC_CALLBACK_1(ResultScene::clickLoginBtn, this));

	loginBtn->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(ResultScene::menuCloseCallback, this));
    
    
	auto menu = Menu::create(loginBtn, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 100, TAG_MENU);

    GameClient::GetInstance().currentScene = RESULT_SCENE_NOW;
    
    return true;
}

void ResultScene::menuCloseCallback(Ref* pSender)
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

void ResultScene::clickLoginBtn(Ref* pSender)
{
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->lobbySendFirstConnectReq();

}