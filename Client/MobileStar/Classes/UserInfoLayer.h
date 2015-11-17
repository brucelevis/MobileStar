
#ifndef __USER_INFO_LAYER_H__
#define __USER_INFO_LAYER_H__

#include "cocos2d.h"
#include "BasicPacket.h"

USING_NS_CC;


class UserInfoLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    ~UserInfoLayer();
    void SetUserInfo(int nickNameLen, const char* nickName);

//	cocos2d::Sprite* backgroundImage;
//	void menuCloseCallback(cocos2d::Ref* pSender);
//	void userInfoLayerClose(cocos2d::Ref* pSender);
//	void doClick1(cocos2d::Ref* pSender);
//	void doClick2(cocos2d::Ref* pSender);
//	void doClick3(cocos2d::Ref* pSender);
//
//	void ClickUserLabel(cocos2d::Object* pSender, cocos2d::LabelTTF* label);
  
	CREATE_FUNC(UserInfoLayer);
    

    
    
    void clickMe(cocos2d::Ref* pSender);

    int64_t userNo;
    int layerPosition;
    std::string nickName;
};

#endif //__USER_LIST_LAYER_H__