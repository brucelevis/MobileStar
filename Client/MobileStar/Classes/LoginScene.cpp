#include "LoginScene.h"
//#include "BackGroundLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "GameClient.h"
#include "CreateAccountLayer.h"
#include "MiniMapLayer.h"



#include "ControlLayer.h"

const char* userId = "12322";

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
		"login_btn_01.png",
		"login_btn_01.png",
		CC_CALLBACK_1(LoginScene::clickLoginBtn, this));

	loginBtn->setPosition(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4);
    
	menu = Menu::create(loginBtn, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 100, TAG_MENU);
    
    createAccountLayer = CreateAccountLayer::create();
    createAccountLayer->retain();
    
    GameClient::GetInstance().currentScene = LOGIN_SCENE_NOW;

//    auto cl = ControlLayer::create();
//    cl->setAnchorPoint(Vec2(0,0));
//    cl->setPosition(Vec2(0,0));
//    addChild(cl, 10000);
    
    
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
    menu->setEnabled(false); //TODO. erase and test
    std::string loginToken = UserDefault::getInstance()->getStringForKey("loginToken");

    if(loginToken.length() == 0)
    {
        char time[37] = {0, };
        memcpy(time, &GameClient::GetInstance().sessionId, 36);
        
        loginToken = std::string(time, 36);
        
        UserDefault::getInstance()->setStringForKey("loginToken", loginToken);
        
        loginToken = UserDefault::getInstance()->getStringForKey("loginToken");

    }

    CCLOG("not have id - %s %d\n", loginToken.c_str(), loginToken.length());

    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendLoginReq(loginToken.c_str(), loginToken.length());
    
}

void LoginScene::openCreateAccountLayer()
{
    menu->setEnabled(false);
    
    createAccountLayer->setAnchorPoint(Vec2(0,0));
    createAccountLayer->setPosition(Vec2(0,0));
    
    addChild(createAccountLayer, 100, TAG_CREATE_ACCOUNT_LAYER);
}

void LoginScene::closeCreateAccountLayer()
{
    menu->setEnabled(true);
    
    removeChildByTag(TAG_CREATE_ACCOUNT_LAYER, false);
}


void LoginScene::createAccountFail(int failReason)
{
    CCLOG("caf");
}

