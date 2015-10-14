#include "LobbyChannelLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "BasicDefines.h"
#include "GameClient.h"
#include "UserListLayer.h"
#include "LobbyScene.h"

USING_NS_CC;


bool LobbyChannelLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    editbox = cocos2d::ui::EditBox::create(Size(620, 80), Scale9Sprite::create("typefield_01.png"));
    editbox->setAnchorPoint(Vec2(0,0));
    editbox->setPosition(Vec2(0, 0));
    editbox->setPlaceHolder("CHATTING");                        //처음 보여주는 문자열
    editbox->setMaxLength(255);                                //최대로 입력받을수 잇는 문자의 길이
    editbox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    editbox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    editbox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    editbox->setInputMode(EditBox::InputMode::ANY);
//    editbox->setInputFlag(EditBox::InputFlag::PASSWORD);

    this->addChild(editbox);
    
    
    userListBtn = MenuItemImage::create(
                                       "user_list_btn_01.png",
                                       "user_list_btn_01.png",
                                       CC_CALLBACK_1(LobbyChannelLayer::clickUserListBtn, this));
    
    chattingSendBtn = MenuItemImage::create(
                                        "chatting_send_btn_01.png",
                                        "chatting_send_btn_01.png",
                                        CC_CALLBACK_1(LobbyChannelLayer::clickChattingSendBtn, this));
    
    userListBtn->setAnchorPoint(Vec2(0, 0));
    chattingSendBtn->setAnchorPoint(Vec2(0, 0));
    
    
    userListBtn->setPosition(Vec2(0, LOBBY_LAYER_HEIGHT - 120));
    chattingSendBtn->setPosition(Vec2(DISPLAY_WIDTH - 100, 0));

    menu = Menu::create(userListBtn, chattingSendBtn, NULL);
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    
    this->addChild(menu, 100, TAG_MENU);

    
    userListLayer = UserListLayer::create();
    userListLayer->retain();
    userListLayer->setAnchorPoint(Vec2(0,0));
    userListLayer->setPosition(Vec2(0,0));
    
    
    
    scrollView = cocos2d::ui::ScrollView::create();
    scrollView->retain();
    
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setInnerContainerSize(Size(DISPLAY_WIDTH, 40 * 20));
    scrollView->setContentSize(Size(DISPLAY_WIDTH, LOBBY_LAYER_HEIGHT - 210));
    scrollView->setBounceEnabled(false);
    scrollView->setAnchorPoint(Vec2(0, 0));
//    scrollView->setBackGroundImage("user_list_back_ground.png");
    
    scrollView->setPosition(Vec2(0, LOBBY_LAYER_Y));
    
    for(int i = 0; i < 20; i++)
    {
        auto chattingLabel = Label::create("", "fonts/nanum.ttf", 36);
        chattingLabel->setAnchorPoint(Vec2(0,0));
        chattingLabel->setPosition(Vec2(0, 40 * i));
        scrollView->addChild(chattingLabel, 1, i);
    }

    
    this->addChild(scrollView, 99);

    scrollView->jumpToBottom();

    rotateCount = 20;
    
    return true;
}

void LobbyChannelLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void LobbyChannelLayer::clickUserListBtn(cocos2d::Ref* pSender)
{
    CCLOG("clickUserListBtn");
    getParent()->addChild(userListLayer, 100, TAG_USER_LIST_LAYER);
}



void LobbyChannelLayer::clickChattingSendBtn(cocos2d::Ref *pSender)
{
    CCLOG("clickChattingSendBtn");
    
    const char* text = editbox->getText();
    
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->chattingSendSendChattingReq(text, strlen(text));
    
    editbox->setText("");
}

void LobbyChannelLayer::notifyChatting(char* nickName, int nickNameLen, char* chatting, int chattingLen)
{
    std::string nickNameStr(nickName, nickNameLen);
    std::string chattingStr(chatting, chattingLen);
    
    
    char chattingBuffer[300];
    memset(chattingBuffer, 0, 300);
    
    sprintf(chattingBuffer, "[%s] : %s", nickNameStr.c_str(), chattingStr.c_str());
    
    for(int i = 0; i < 19; i++)
    {
        auto chattingLabel = scrollView->getChildByTag(indexToTagNum(i));
        chattingLabel->setPosition(Vec2(0, 40 * (i + 1)));

    }
    
    Label* chattingLabel = (Label*)scrollView->getChildByTag(indexToTagNum(19));
    chattingLabel->setPosition(Vec2(0, 0));
    
    chattingLabel->setString(std::string(chattingBuffer));
    
    
    rotateCount--;
    if(rotateCount == 0)
        rotateCount = 20;
}

int LobbyChannelLayer::indexToTagNum(int i)
{
    return ((rotateCount + i) % 20);
}

void LobbyChannelLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    CCLOG("editBoxEditingDidBegin");
    
}


void LobbyChannelLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    CCLOG("editBoxEditingDidBegin");
    clickChattingSendBtn(this);

}


void LobbyChannelLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    CCLOG("editBoxTextChanged");

    clickChattingSendBtn(this);
}


void LobbyChannelLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    CCLOG("editBoxReturn");

}


