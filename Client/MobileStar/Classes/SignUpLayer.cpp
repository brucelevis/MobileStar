#include "SignUpLayer.h"
#include "LoginScene.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "GameClient.h"
#include "MiniMapLayer.h"


USING_NS_CC;

bool SignUpLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto bg = Sprite::create("sign_up_back_ground.png");
    bg->setAnchorPoint(Vec2(0,0));
    bg->setPosition(Vec2(0, 0));
    
    this->addChild(bg);
    
    
    auto idLabel = Sprite::create("id_label.png");
    idLabel->setAnchorPoint(Vec2(0,0));
    idLabel->setPosition(Vec2(60, 610));
    
    this->addChild(idLabel);
    
    idEditBox = cocos2d::ui::EditBox::create(Size(420, 80), Scale9Sprite::create("typefield_01.png"));
    idEditBox->setAnchorPoint(Vec2(0,0));
    idEditBox->setPosition(Vec2(250, 610));
    idEditBox->setPlaceHolder("ID");                        //처음 보여주는 문자열
    idEditBox->setMaxLength(20);                                //최대로 입력받을수 잇는 문자의 길이
    idEditBox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    idEditBox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    idEditBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    idEditBox->setInputMode(EditBox::InputMode::ANY);
    
    this->addChild(idEditBox);

    
    auto passwordLabel = Sprite::create("password_label.png");
    passwordLabel->setAnchorPoint(Vec2(0,0));
    passwordLabel->setPosition(Vec2(60, 480));
    
    this->addChild(passwordLabel);
    
    passwordEditBox = cocos2d::ui::EditBox::create(Size(420, 80), Scale9Sprite::create("typefield_01.png"));
    passwordEditBox->setAnchorPoint(Vec2(0,0));
    passwordEditBox->setPosition(Vec2(250, 480));
    passwordEditBox->setPlaceHolder("PASSWORD");                        //처음 보여주는 문자열
    passwordEditBox->setMaxLength(20);                                //최대로 입력받을수 잇는 문자의 길이
    passwordEditBox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    passwordEditBox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    passwordEditBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    passwordEditBox->setInputMode(EditBox::InputMode::ANY);
    passwordEditBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    
    this->addChild(passwordEditBox);

    
    auto confirmPasswordLabel = Sprite::create("confirm_password_label.png");
    confirmPasswordLabel->setAnchorPoint(Vec2(0,0));
    confirmPasswordLabel->setPosition(Vec2(60, 350));
    
    this->addChild(confirmPasswordLabel);
    
    confirmPasswordEditBox = cocos2d::ui::EditBox::create(Size(420, 80), Scale9Sprite::create("typefield_01.png"));
    confirmPasswordEditBox->setAnchorPoint(Vec2(0,0));
    confirmPasswordEditBox->setPosition(Vec2(250, 350));
    confirmPasswordEditBox->setPlaceHolder("CONFIRM P.W");                        //처음 보여주는 문자열
    confirmPasswordEditBox->setMaxLength(20);                                //최대로 입력받을수 잇는 문자의 길이
    confirmPasswordEditBox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    confirmPasswordEditBox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    confirmPasswordEditBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    confirmPasswordEditBox->setInputMode(EditBox::InputMode::ANY);
    confirmPasswordEditBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    
    this->addChild(confirmPasswordEditBox);

    auto emailLabel = Sprite::create("email_label.png");
    emailLabel->setAnchorPoint(Vec2(0,0));
    emailLabel->setPosition(Vec2(60, 220));
    
    this->addChild(emailLabel);
    
    emailEditBox = cocos2d::ui::EditBox::create(Size(420, 80), Scale9Sprite::create("typefield_01.png"));
    emailEditBox->setAnchorPoint(Vec2(0,0));
    emailEditBox->setPosition(Vec2(250, 220));
    emailEditBox->setPlaceHolder("EMAIL");                        //처음 보여주는 문자열
    emailEditBox->setMaxLength(20);                                //최대로 입력받을수 잇는 문자의 길이
    emailEditBox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    emailEditBox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    emailEditBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    emailEditBox->setInputMode(EditBox::InputMode::ANY);
    
    this->addChild(emailEditBox);

    
    
    auto makeBtn = MenuItemImage::create(
                                        "sign_up_btn_01.png",
                                        "sign_up_btn_02.png",
                                        CC_CALLBACK_1(SignUpLayer::clickMakeBtn, this));
    
    auto signUpCancelBtn = MenuItemImage::create(
                                            "sign_up_cancel_btn_01.png",
                                            "sign_up_cancel_btn_02.png",
                                            CC_CALLBACK_1(SignUpLayer::clickCancelBtn, this));
    
    makeBtn->setAnchorPoint(Vec2(0, 0));
    signUpCancelBtn->setAnchorPoint(Vec2(0, 0));
    
    
    makeBtn->setPosition(Vec2(120, 60));
    signUpCancelBtn->setPosition(Vec2(DISPLAY_WIDTH - 320, 60));
    
    menu = Menu::create(makeBtn, signUpCancelBtn, NULL);
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    
    this->addChild(menu, 100, TAG_MENU);

    
    return true;
}

void SignUpLayer::menuCloseCallback(Ref* pSender)
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

void SignUpLayer::clickMakeBtn(Ref* pSender)
{
    menu->setEnabled(false);
    
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendFirstConnectReq();
}


void SignUpLayer::clickCancelBtn(Ref* pSender)
{
    ((LoginScene*)getParent())->removeSignUpLayer();
}


void SignUpLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    
}


void SignUpLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}


void SignUpLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}


void SignUpLayer::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}
