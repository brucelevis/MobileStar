#ifndef __MENU_BUTTON_LAYER_H__
#define __MENU_BUTTON_LAYER_H__

#include "cocos2d.h"

#define MENU_BUTTON_WIDTH 120
#define MENU_BUTTON_HEIGHT 120


class MenuButtonLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void clickGameBtn(cocos2d::Ref* pSender);
    void clickChannelBtn(cocos2d::Ref* pSender);
    void clickFriendBtn(cocos2d::Ref* pSender);
    void clickClanBtn(cocos2d::Ref* pSender);
    void clickRankingBtn(cocos2d::Ref* pSender);
    void clickBoardBtn(cocos2d::Ref* pSender);

	CREATE_FUNC(MenuButtonLayer);
    
private:
    cocos2d::Menu* menu;
};

#endif //__MENU_BUTTON_LAYER_H__
