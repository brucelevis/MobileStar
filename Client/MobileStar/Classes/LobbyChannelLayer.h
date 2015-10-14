#ifndef __LOBBY_CHANNEL_LAYER_H__
#define __LOBBY_CHANNEL_LAYER_H__

#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <ui/UIScrollView.h>

USING_NS_CC;

using namespace ui;

class UserListLayer;

class LobbyChannelLayer : public cocos2d::Layer, cocos2d::ui::EditBoxDelegate
{
public:
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void clickUserListBtn(cocos2d::Ref* pSender);
    void clickChattingSendBtn(cocos2d::Ref* pSender);
    void notifyChatting(char* nickName, int nickNameLen, char* chatting, int chattingLen);
    int indexToTagNum(int i);


	CREATE_FUNC(LobbyChannelLayer);
    
    
protected:
    void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
public:
    cocos2d::Menu* menu;
    
    cocos2d::ui::EditBox* editbox;
    cocos2d::MenuItemImage* userListBtn;
    cocos2d::MenuItemImage* chattingSendBtn;
    
    UserListLayer* userListLayer;
        
    cocos2d::ui::ScrollView* scrollView;
    
    int rotateCount;
};

#endif //__LOBBY_CHANNEL_LAYER_H__
