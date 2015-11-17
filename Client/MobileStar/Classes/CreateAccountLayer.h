#ifndef __CREATE_ACCOUNT_LAYER_H__
#define __CREATE_ACCOUNT_LAYER_H__

#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <ui/UIScrollView.h>

USING_NS_CC;

using namespace ui;

class CreateAccountLayer : public cocos2d::Layer, cocos2d::ui::EditBoxDelegate
{
public:
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void clickOkBtn(cocos2d::Ref* pSender);
    void clickCancelBtn(cocos2d::Ref* pSender);
    void notifyChatting(char* nickName, int nickNameLen, char* chatting, int chattingLen);
    int indexToTagNum(int i);


	CREATE_FUNC(CreateAccountLayer);
    
    
protected:
    void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
public:
    cocos2d::Menu* menu;
    
    cocos2d::ui::EditBox* editbox;
    cocos2d::MenuItemImage* okBtn;
    cocos2d::MenuItemImage* cancelBtn;
};

#endif //__CREATE_ACCOUNT_LAYER_H__
