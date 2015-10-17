#include "LoginScene.h"
#include "SignUpLayer.h"
//#include "BackGroundLayer.h"
#include "GameDefines.h"
#include "ClientFrontPacket.h"
#include "NetworkLayer.h"
#include "GameClient.h"
#include "MiniMapLayer.h"




#include "ControlLayer.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();

	auto layer = LoginScene::create();
	
	scene->addChild(layer, 1, TAG_LOGIN_SCENE);

    return scene;
}

bool LoginScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto idLabel = Sprite::create("id_label.png");
    idLabel->setAnchorPoint(Vec2(0,0));
    idLabel->setPosition(Vec2(60, 350));
    
    this->addChild(idLabel);
    
    idEditBox = cocos2d::ui::EditBox::create(Size(420, 80), Scale9Sprite::create("typefield_01.png"));
    idEditBox->setAnchorPoint(Vec2(0,0));
    idEditBox->setPosition(Vec2(250, 350));
    idEditBox->setPlaceHolder("ID");                        //처음 보여주는 문자열
    idEditBox->setMaxLength(20);                                //최대로 입력받을수 잇는 문자의 길이
    idEditBox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    idEditBox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    idEditBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    idEditBox->setInputMode(EditBox::InputMode::ANY);
    
    this->addChild(idEditBox);

    
    auto passwordLabel = Sprite::create("password_label.png");
    passwordLabel->setAnchorPoint(Vec2(0,0));
    passwordLabel->setPosition(Vec2(60, 220));
    
    this->addChild(passwordLabel);
    
    passwordEditBox = cocos2d::ui::EditBox::create(Size(420, 80), Scale9Sprite::create("typefield_01.png"));
    passwordEditBox->setAnchorPoint(Vec2(0,0));
    passwordEditBox->setPosition(Vec2(250, 220));
    passwordEditBox->setPlaceHolder("PASSWORD");                        //처음 보여주는 문자열
    passwordEditBox->setMaxLength(20);                                //최대로 입력받을수 잇는 문자의 길이
    passwordEditBox->setPlaceholderFontColor(Color3B::WHITE);        //문자열 색변경
    passwordEditBox->setFontColor(Color3B::BLACK);                    //입력한 문자열 색변경
    passwordEditBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    passwordEditBox->setInputMode(EditBox::InputMode::ANY);
    passwordEditBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    
    this->addChild(passwordEditBox);
    
    
    
    auto loginBtn = MenuItemImage::create(
                                        "login_btn_01.png",
                                        "login_btn_02.png",
                                        CC_CALLBACK_1(LoginScene::clickLoginBtn, this));
    
    auto signUpBtn = MenuItemImage::create(
                                            "sign_up_btn_01.png",
                                            "sign_up_btn_02.png",
                                            CC_CALLBACK_1(LoginScene::clickSignUpBtn, this));
    
    loginBtn->setAnchorPoint(Vec2(0, 0));
    signUpBtn->setAnchorPoint(Vec2(0, 0));
    
    
    loginBtn->setPosition(Vec2(120, 60));
    signUpBtn->setPosition(Vec2(DISPLAY_WIDTH - 320, 60));
    
    menu = Menu::create(loginBtn, signUpBtn, NULL);
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    
    this->addChild(menu, 100, TAG_MENU);
    
    signUpLayer = SignUpLayer::create();
    signUpLayer->retain();
    

    
    GameClient::GetInstance().currentScene = LOGIN_SCENE_NOW;

    menu->setEnabled(false);
    idEditBox->setEnabled(false);
    passwordEditBox->setEnabled(false);
    
    ((NetworkLayer*)getParent()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendFirstConnectReq();
    
//    auto cl = ControlLayer::create();
//    cl->setAnchorPoint(Vec2(0,0));
//    cl->setPosition(Vec2(0,0));
//    addChild(cl, 10000);
    
    
    return true;
}

void LoginScene::menuCloseCallback(Ref* pSender)
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

void LoginScene::clickLoginBtn(Ref* pSender)
{
    menu->setEnabled(false);
    idEditBox->setEnabled(false);
    passwordEditBox->setEnabled(false);
    
    const char* nickName = idEditBox->getText();
    const char* password = passwordEditBox->getText();
    
    
    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendLoginReq(nickName, strlen(nickName), password, strlen(password));
}


void LoginScene::clickSignUpBtn(Ref* pSender)
{
    menu->setEnabled(false);
    idEditBox->setEnabled(false);
    passwordEditBox->setEnabled(false);
    
    addChild(signUpLayer, 1000, TAG_SIGN_UP_LAYER);
//    ((NetworkLayer*)Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER))->handler->frontSendFirstConnectReq();
}

void LoginScene::completeFirstConnect()
{
    menu->setEnabled(true);
    idEditBox->setEnabled(true);
    passwordEditBox->setEnabled(true);
}

void LoginScene::completeSignUp()
{
    idEditBox->setText(signUpLayer->idEditBox->getText());
    passwordEditBox->setText(signUpLayer->passwordEditBox->getText());
    
    removeChildByTag(TAG_SIGN_UP_LAYER);
    
    menu->setEnabled(true);
    idEditBox->setEnabled(true);
    passwordEditBox->setEnabled(true);
}

void LoginScene::removeSignUpLayer()
{
    removeChildByTag(TAG_SIGN_UP_LAYER);
    
    menu->setEnabled(true);
    idEditBox->setEnabled(true);
    passwordEditBox->setEnabled(true);
}


void LoginScene::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    
}


void LoginScene::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}


void LoginScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
}


void LoginScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    
}
