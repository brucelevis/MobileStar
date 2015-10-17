#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <ui/UIScrollView.h>

USING_NS_CC;

using namespace ui;

class SignUpLayer;

class LoginScene : public cocos2d::Layer, cocos2d::ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void clickLoginBtn(cocos2d::Ref* pSender);
    void clickSignUpBtn(cocos2d::Ref* pSender);
    
    void completeFirstConnect();
    void completeSignUp();
    void removeSignUpLayer();
    

	CREATE_FUNC(LoginScene);
    
protected:
    void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
private:
    cocos2d::Menu* menu;
    
    cocos2d::ui::EditBox* idEditBox;
    cocos2d::ui::EditBox* passwordEditBox;
    
    
    SignUpLayer* signUpLayer;
};

#endif //__FRONT_SCENE_H__
