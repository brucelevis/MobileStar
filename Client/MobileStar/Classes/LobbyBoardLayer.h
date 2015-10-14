#ifndef __LOBBY_BOARD_LAYER_H__
#define __LOBBY_BOARD_LAYER_H__

#include "cocos2d.h"

#define MENU_BUTTON_WIDTH 120
#define MENU_BUTTON_HEIGHT 120


class LobbyBoardLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
   
    /*
    void clickGameBtn(cocos2d::Ref* pSender);
    void clickChannelBtn(cocos2d::Ref* pSender);
    void clickFriendBtn(cocos2d::Ref* pSender);
    void clickClanBtn(cocos2d::Ref* pSender);
    void clickRankingBtn(cocos2d::Ref* pSender);
    void clickBoardBtn(cocos2d::Ref* pSender);
    
    void clickChina(cocos2d::Ref* pSender);
    void clickEurope(cocos2d::Ref* pSender);
    void clickUSA(cocos2d::Ref* pSender);
    
    void clickAIGame(cocos2d::Ref* pSender);
    void clickCommonGame(cocos2d::Ref* pSender);
    void clickRankGame(cocos2d::Ref* pSender);
    */

	CREATE_FUNC(LobbyBoardLayer);
    
private:
    cocos2d::Menu* menu;
    
    cocos2d::MenuItemImage* tribeChina;
    cocos2d::MenuItemImage* tribeEurope;
    cocos2d::MenuItemImage* tribeUSA;
    
    cocos2d::MenuItemImage* AIGame;
    cocos2d::MenuItemImage* commonGame;
    cocos2d::MenuItemImage* rankGame;
    
};

#endif //__LOBBY_GAME_LAYER_H__
