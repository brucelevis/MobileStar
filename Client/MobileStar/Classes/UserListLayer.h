
#ifndef __USER_LIST_LAYER_H__
#define __USER_LIST_LAYER_H__

#include "cocos2d.h"
#include "UserInfoLayer.h"
#include <vector>

#include <ui/UIScrollView.h>

#define CONTAINER_HEIGHT DISPLAY_HEIGHT * 2

class UserListLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    void addUserViewInfo(int nickNameLen, const char* nickName);
    void removeUserViewInfo(int nickNameLen, const char* nickName);
    
    void clickUserInfo(cocos2d::Ref* pSender);
    void clickCloseBtn(cocos2d::Ref* pSender);


//	void SetUserList(int nickNameCount, NickNameInfo* nickNameInfo);
//	void SetUserInfo(UserInfo* userInfo);
//
//	cocos2d::Sprite* backgroundImage;
//	void userInfoLayerClose(cocos2d::Ref* pSender);
//	void doClick2(cocos2d::Ref* pSender);
//	void doClick3(cocos2d::Ref* pSender);

//	void ClickUserLabel(cocos2d::Object* pSender, cocos2d::LabelTTF* label);
    virtual bool onTouchBegan(Touch* touch, Event* _event);
    virtual void onTouchMoved(Touch* touch, Event* _event);
    virtual void onTouchCancelled(Touch* touch, Event* _event);
    virtual void onTouchEnded(Touch* touch, Event *_event);
    
	CREATE_FUNC(UserListLayer);

	cocos2d::Menu* menu;
	cocos2d::MenuItemImage* closeItem;
    
    int userCount;
    int nextAddUserNumber;
    int downCount;
    
    std::vector<UserInfoLayer*> userInfoLayerList;
//    UserInfoLayer* userInfoLayerList[MAX_USER_COUNT_IN_CHANNEL];
    
    cocos2d::ui::ScrollView* scrollView;
};

#endif //__USER_LIST_LAYER_H__