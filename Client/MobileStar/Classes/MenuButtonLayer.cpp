#include "MenuButtonLayer.h"
#include "GameDefines.h"
#include "NetworkLayer.h"
#include "BasicDefines.h"
#include "LobbyScene.h"

USING_NS_CC;


bool MenuButtonLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto gameBtn = MenuItemImage::create(
                                         "menu_game_btn_01.png",
                                         "menu_game_btn_02.png",
                                         CC_CALLBACK_1(MenuButtonLayer::clickGameBtn, this));
    
    gameBtn->setAnchorPoint(Vec2(0,0));
    gameBtn->setPosition(0, 0);
    
    auto channelBtn = MenuItemImage::create(
                                         "menu_channel_btn_01.png",
                                         "menu_channel_btn_02.png",
                                         CC_CALLBACK_1(MenuButtonLayer::clickChannelBtn, this));
    
    channelBtn->setAnchorPoint(Vec2(0,0));
    channelBtn->setPosition(MENU_BUTTON_WIDTH * 1, 0);
    
    auto friendBtn = MenuItemImage::create(
                                         "menu_friend_btn_01.png",
                                         "menu_friend_btn_02.png",
                                         CC_CALLBACK_1(MenuButtonLayer::clickFriendBtn, this));
    
    friendBtn->setAnchorPoint(Vec2(0,0));
    friendBtn->setPosition(MENU_BUTTON_WIDTH * 2, 0);
    
    auto clanBtn = MenuItemImage::create(
                                         "menu_clan_btn_01.png",
                                         "menu_clan_btn_02.png",
                                         CC_CALLBACK_1(MenuButtonLayer::clickClanBtn, this));
    
    clanBtn->setAnchorPoint(Vec2(0,0));
    clanBtn->setPosition(MENU_BUTTON_WIDTH * 3, 0);
    
    auto rankingBtn = MenuItemImage::create(
                                         "menu_ranking_btn_01.png",
                                         "menu_ranking_btn_02.png",
                                         CC_CALLBACK_1(MenuButtonLayer::clickRankingBtn, this));
    
    rankingBtn->setAnchorPoint(Vec2(0,0));
    rankingBtn->setPosition(MENU_BUTTON_WIDTH * 4, 0);
    
    auto boardBtn = MenuItemImage::create(
                                         "menu_board_btn_01.png",
                                         "menu_board_btn_02.png",
                                         CC_CALLBACK_1(MenuButtonLayer::clickBoardBtn, this));
    
    boardBtn->setAnchorPoint(Vec2(0,0));
    boardBtn->setPosition(MENU_BUTTON_WIDTH * 5, 0);
    
	menu = Menu::create(gameBtn, channelBtn, friendBtn, clanBtn, rankingBtn, boardBtn, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 100, TAG_MENU);

    return true;
}

void MenuButtonLayer::menuCloseCallback(Ref* pSender)
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


void MenuButtonLayer::clickGameBtn(cocos2d::Ref* pSender)
{
    ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->changeLobbyLayer(CURRENT_LAYER_GAME);
}


void MenuButtonLayer::clickChannelBtn(cocos2d::Ref* pSender)
{
    ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->changeLobbyLayer(CURRENT_LAYER_CHANNEL);
}


void MenuButtonLayer::clickFriendBtn(cocos2d::Ref* pSender)
{
        ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->changeLobbyLayer(CURRENT_LAYER_FRIEND);
}


void MenuButtonLayer::clickClanBtn(cocos2d::Ref* pSender)
{
        ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->changeLobbyLayer(CURRENT_LAYER_CLAN);
}


void MenuButtonLayer::clickRankingBtn(cocos2d::Ref* pSender)
{
        ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->changeLobbyLayer(CURRENT_LAYER_RANKING);
}


void MenuButtonLayer::clickBoardBtn(cocos2d::Ref* pSender)
{
        ((LobbyScene*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_LOBBY_SCENE))->changeLobbyLayer(CURRENT_LAYER_BOARD);
}

