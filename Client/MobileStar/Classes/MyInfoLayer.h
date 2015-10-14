#ifndef __MY_INFO_LAYER_LAYER_H__
#define __MY_INFO_LAYER_LAYER_H__

#include "cocos2d.h"

#define MENU_BUTTON_WIDTH 120
#define MENU_BUTTON_HEIGHT 120


class MyInfoLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void clickGameBtn(cocos2d::Ref* pSender);

	CREATE_FUNC(MyInfoLayer);
    
private:
    cocos2d::Menu* menu;
};

#endif //__MY_INFO_LAYER_LAYER_H__
