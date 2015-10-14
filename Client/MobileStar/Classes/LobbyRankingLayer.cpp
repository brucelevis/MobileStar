#include "LobbyRankingLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "BasicDefines.h"
#include "GameClient.h"

USING_NS_CC;


bool LobbyRankingLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    /*
    tribeChina = MenuItemImage::create(
                                       "china_btn_01.png",
                                       "china_btn_01.png",
                                       CC_CALLBACK_1(LobbyGameLayer::clickChina, this));
    
    tribeEurope = MenuItemImage::create(
                                        "europe_btn_01.png",
                                        "europe_btn_01.png",
                                        CC_CALLBACK_1(LobbyGameLayer::clickEurope, this));
    
    tribeUSA = MenuItemImage::create(
                                     "usa_btn_01.png",
                                     "usa_btn.png",
                                     CC_CALLBACK_1(LobbyGameLayer::clickUSA, this));

    
    AIGame = MenuItemImage::create(
                                   "ai_game_btn_01.png",
                                   "ai_game_btn_01.png",
                                   CC_CALLBACK_1(LobbyGameLayer::clickAIGame, this));
    
    commonGame = MenuItemImage::create(
                                   "common_game_btn_01.png",
                                   "common_game_btn_01.png",
                                   CC_CALLBACK_1(LobbyGameLayer::clickCommonGame, this));
    
    rankGame = MenuItemImage::create(
                                   "rank_game_btn_01.png",
                                   "rank_game_btn_01.png",
                                   CC_CALLBACK_1(LobbyGameLayer::clickRankGame, this));
    
    tribeChina->setAnchorPoint(Vec2(0, 0));
    tribeEurope->setAnchorPoint(Vec2(0, 0));
    tribeUSA->setAnchorPoint(Vec2(0, 0));
    AIGame->setAnchorPoint(Vec2(0, 0));
    commonGame->setAnchorPoint(Vec2(0, 0));
    rankGame->setAnchorPoint(Vec2(0, 0));
    
    
    tribeChina->setPosition(Vec2(0, LOBBY_LAYER_HEIGHT - 160));
    tribeEurope->setPosition(Vec2(240, LOBBY_LAYER_HEIGHT - 160));
    tribeUSA->setPosition(Vec2(480, LOBBY_LAYER_HEIGHT - 160));
    AIGame->setPosition(Vec2(0, LOBBY_LAYER_HEIGHT - 160 - 180));
    commonGame->setPosition(Vec2(0, LOBBY_LAYER_HEIGHT - 160 - 180 - 180));
    rankGame->setPosition(Vec2(0, LOBBY_LAYER_HEIGHT - 160 - 180 - 180 - 180));

    menu = Menu::create(tribeChina, tribeEurope, tribeUSA, AIGame, commonGame, rankGame, NULL);
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    
    this->addChild(menu, 100, TAG_MENU);
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_CHINA;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeChina->addChild(choose, 0, 123);
*/
    return true;
}
/*
void LobbyGameLayer::menuCloseCallback(Ref* pSender)
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


void LobbyGameLayer::clickGameBtn(cocos2d::Ref* pSender)
{
//    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendFirstConnectReq();
    
}



void LobbyGameLayer::clickChina(cocos2d::Ref *pSender)
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

    }
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_CHINA;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeChina->addChild(choose, 1, 123);
}
void LobbyGameLayer::clickEurope(cocos2d::Ref *pSender)
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

    }
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_EUROPE;
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    tribeEurope->addChild(choose, 1, 123);
}
void LobbyGameLayer::clickUSA(cocos2d::Ref *pSender)
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

    }
    
    auto choose = Sprite::create("choose_tribe.png");
    choose->setAnchorPoint(Vec2::ZERO);
    
    
    GameClient::GetInstance().tribe = TRIBE_TYPE_USA;
    tribeUSA->addChild(choose, 1, 123);
}



void LobbyGameLayer::clickAIGame(cocos2d::Ref *pSender)
{
//    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->lobbySendQuickPlayReq(GameClient::GetInstance().tribe);
}

void LobbyGameLayer::clickCommonGame(cocos2d::Ref *pSender)
{
//    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->lobbySendQuickPlayReq(GameClient::GetInstance().tribe);
}

void LobbyGameLayer::clickRankGame(cocos2d::Ref *pSender)
{
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->lobbySendQuickPlayReq(GameClient::GetInstance().tribe);
}
*/
