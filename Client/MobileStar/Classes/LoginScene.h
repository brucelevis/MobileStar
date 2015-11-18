#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"


class CreateAccountLayer;

class LoginScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
	void clickLoginBtn(cocos2d::Ref* pSender);
    void openCreateAccountLayer();
    void closeCreateAccountLayer();
    void createAccountFail(int failReason);
    

	CREATE_FUNC(LoginScene);
    
private:
    cocos2d::Menu* menu;
    
    CreateAccountLayer* createAccountLayer;
    
};

#endif //__FRONT_SCENE_H__
