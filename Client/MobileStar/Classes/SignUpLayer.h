#ifndef __SIGN_UP_LAYER_H__
#define __SIGN_UP_LAYER_H__

#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <ui/UIScrollView.h>

USING_NS_CC;

using namespace ui;

class SignUpLayer : public cocos2d::Layer, cocos2d::ui::EditBoxDelegate
{
public:
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void clickMakeBtn(cocos2d::Ref* pSender);
    void clickCancelBtn(cocos2d::Ref* pSender);
    

	CREATE_FUNC(SignUpLayer);
    
    friend class LoginScene;
    
protected:
    void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
private:
    cocos2d::Menu* menu;
    
    cocos2d::ui::EditBox* idEditBox;
    cocos2d::ui::EditBox* passwordEditBox;
    cocos2d::ui::EditBox* confirmPasswordEditBox;
    cocos2d::ui::EditBox* emailEditBox;
    
};

#endif //__SIGN_UP_LAYER_H__
