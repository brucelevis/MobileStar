
#ifndef __USER_LIST_LAYER_H__
#define __USER_LIST_LAYER_H__

#include "cocos2d.h"
#include "UserInfoLayer.h"

class UserListLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    void addNextUserList(int64_t userNo, int nickNameLen, const char* nickName);
    void addPrevUserList(int64_t userNo, int nickNameLen, const char* nickName);

    void ClickUserInfo(cocos2d::Ref* pSender);


//	void SetUserList(int nickNameCount, NickNameInfo* nickNameInfo);
//	void SetUserInfo(UserInfo* userInfo);
//
//	cocos2d::Sprite* backgroundImage;
//	void menuCloseCallback(cocos2d::Ref* pSender);
//	void userInfoLayerClose(cocos2d::Ref* pSender);
//	void doClick2(cocos2d::Ref* pSender);
//	void doClick3(cocos2d::Ref* pSender);

//	void ClickUserLabel(cocos2d::Object* pSender, cocos2d::LabelTTF* label);
    
    
	CREATE_FUNC(UserListLayer);

	cocos2d::Menu* menu;
	cocos2d::MenuItemImage* closeItem;
    
    int userCount;
    int nextAddUserNumber;
    int downCount;
    
    UserInfoLayer* userInfoLayerList[MAX_USER_COUNT_IN_CHANNEL + 10];
};

#endif //__USER_LIST_LAYER_H__